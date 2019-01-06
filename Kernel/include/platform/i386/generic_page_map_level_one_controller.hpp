#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_GENERIC_PAGE_MAP_LEVEL_ONE_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_GENERIC_PAGE_MAP_LEVEL_ONE_CONTROLLER_HPP

#include <paging_topology/page_map_level_one_controller.hpp>
#include <hardware_memory_management_unit.hpp>
#include <common/unique_pointer.hpp>
#include <common/mutex.hpp>
#include <cassert>
#include <type_traits> // for std::remove_reference

namespace i386 {

// Owns a page table. Provides synchronized access.
// On IA-32, the first level page map is called a Page Table.
template<typename Policy>
class GenericPageMapLevelOneController final : public PagingTopology::PageMapLevelOneController {
public:
    using MyPageFrameController = typename Policy::MyPageFrameController;
    using MyPageTable = typename Policy::MyPageTable;
    using MyPageTableEntry = typename std::remove_reference<decltype(MyPageTable::entries[0])>::type;
    static constexpr size_t COUNT = MyPageTable::COUNT;

    // A single entry in the page map.
    // This controls the contents of one underlying Page Table Entry and also
    // owns the page frame which is referenced therein.
    class Entry final : public PagingTopology::PageMapLevelOneController::Entry {
    public:
        virtual ~Entry() = default;

        Entry() : lock_(nullptr), pte_(nullptr) {}

        void set_lock(Mutex* lock)
        {
            assert(lock);
            lock_ = lock;
        }

        void set_pte(MyPageTableEntry* pte)
        {
            assert(pte);
            pte_ = pte;
        }

        SharedPointer<PagingTopology::PageFrameController> get_page_frame() const override
        {
            assert(lock_);
            LockGuard guard{*lock_};
            return page_frame_;
        }
        
        void set_page_frame(SharedPointer<PagingTopology::PageFrameController> p) override
        {
            assert(lock_);
            LockGuard guard{*lock_};
            set_page_frame_unlocked(p);
        }
        
        void set_page_frame_unlocked(SharedPointer<PagingTopology::PageFrameController> p)
        {
            assert(pte_);
            page_frame_ = std::move(p);
            if (has_page_frame_unlocked()) {
                pte_->set_address(page_frame_->get_page_frame());
                pte_->set_present(true);
            } else {
                pte_->set_address(0);
                pte_->set_present(false);
            }
        }

        bool has_page_frame_unlocked() const
        {
            return page_frame_ && (page_frame_->get_page_frame() != 0);
        }
        
        bool is_present() const override
        {
            assert(pte_);
            assert(lock_);
            LockGuard guard{*lock_};
            return pte_->is_present();
        }

        void set_present(bool present) override
        {
            assert(pte_);
            assert(lock_);
            LockGuard guard{*lock_};
            pte_->set_present(present);
        }

        bool is_readwrite() const override
        {
            assert(pte_);
            assert(lock_);
            LockGuard guard{*lock_};
            return pte_->is_readwrite();
        }

        void set_readwrite(bool readwrite) override
        {
            assert(pte_);
            assert(lock_);
            LockGuard guard{*lock_};
            pte_->set_readwrite(readwrite);
        }

        bool is_supervisor() const override
        {
            assert(pte_);
            assert(lock_);
            LockGuard guard{*lock_};
            return pte_->is_supervisor();
        }

        void set_supervisor(bool supervisor) override
        {
            assert(pte_);
            assert(lock_);
            LockGuard guard{*lock_};
            pte_->set_supervisor(supervisor);
        }

        bool is_accessed() const override
        {
            assert(pte_);
            assert(lock_);
            LockGuard guard{*lock_};
            return pte_->is_accessed();
        }

        void set_accessed(bool accessed) override
        {
            assert(pte_);
            assert(lock_);
            LockGuard guard{*lock_};
            pte_->set_accessed(accessed);
        }

        bool is_dirty() const override
        {
            assert(pte_);
            assert(lock_);
            LockGuard guard{*lock_};
            return pte_->is_dirty();
        }

        void set_dirty(bool dirty) override
        {
            assert(pte_);
            assert(lock_);
            LockGuard guard{*lock_};
            pte_->set_dirty(dirty);
        }
        
        bool is_global() const override
        {
            assert(pte_);
            assert(lock_);
            LockGuard guard{*lock_};
            return pte_->is_global();
        }
        
        void set_global(bool global) override
        {
            assert(pte_);
            assert(lock_);
            LockGuard guard{*lock_};
            pte_->set_global(global);
        }

        void set_protection(ProtectionFlags flags) override
        {
            assert(pte_);
            assert(lock_);
            LockGuard guard{*lock_};
            set_protection_unlocked(flags);
        }

        void set_protection_unlocked(ProtectionFlags flags)
        {
            assert(pte_);
            pte_->set_present(true);
            pte_->set_readwrite((flags & WRITABLE) != 0);
            pte_->set_supervisor((flags & SUPERVISOR) != 0);
            pte_->set_global((flags & GLOBAL) != 0);
        }
        
        void set_mapping(uintptr_t physical_address,
                         ProtectionFlags flags) override
        {
            assert(lock_);
            LockGuard guard{*lock_};
            set_page_frame_unlocked(create_page_frame_controller(physical_address));
            set_protection_unlocked(flags);
        }
        
        UniquePointer<PagingTopology::PageFrameController> create_page_frame_controller(uintptr_t page_frame) override
        {
            return new MyPageFrameController{page_frame};
        }

    private:
        Mutex* lock_;
        MyPageTableEntry* pte_;
        SharedPointer<PagingTopology::PageFrameController> page_frame_;
    };

    GenericPageMapLevelOneController(HardwareMemoryManagementUnit& mmu)
     : mmu_(mmu),
       page_table_(new MyPageTable)
    {
        clear_table();
        feed_ptes();
        feed_lock();
    }

    GenericPageMapLevelOneController(HardwareMemoryManagementUnit& mmu,
                                     UniquePointer<MyPageTable> pt)
     : mmu_(mmu),
       page_table_(std::move(pt))
    {
        if (page_table_) {
            feed_ptes();
        }
        feed_lock();
    }

    GenericPageMapLevelOneController(const GenericPageMapLevelOneController&) = delete;
    GenericPageMapLevelOneController(GenericPageMapLevelOneController&&) = delete;

    void clear_table()
    {
        assert(page_table_);
        memset(page_table_.get_pointer(), 0, sizeof(MyPageTable));
    }

    void feed_ptes()
    {
        assert(page_table_);
        for (size_t i = 0; i < COUNT; ++i) {
            auto& pte = page_table_->entries[i];
            entries_[i].set_pte(&pte);
        }
    }

    void feed_lock()
    {
        for (size_t i = 0; i < COUNT; ++i) {
            entries_[i].set_lock(&lock_);
        }
    }

    // Returns the number of entries in the table.
    size_t get_number_of_entries() const override
    {
        return COUNT;
    }

    // Gets the physical address of the underlying page table.
    uintptr_t get_underlying_object_physical_address() const override
    {
        LockGuard guard{lock_};
        MyPageTable* page_table = page_table_.get_pointer();
        if (page_table) {
            uintptr_t logical = reinterpret_cast<uintptr_t>(page_table);
            uintptr_t physical = mmu_.convert_logical_to_physical_address(logical);
            return physical;
        } else {
            return 0;
        }
    }

    // Gets the specified entry in the table.
    Entry& get_entry(size_t index) override
    {
        assert(index < COUNT);
        LockGuard guard{lock_};
        return entries_[index];
    }

    // Gets the specified entry in the table.
    const Entry& get_entry(size_t index) const override
    {
        assert(index < COUNT);
        LockGuard guard{lock_};
        return entries_[index];
    }

    // Gets the size of the region of memory governed by each entry.
    size_t get_size_governed_by_entry() const override
    {
        return PAGE_SIZE;
    }

private:
    mutable Mutex lock_;
    HardwareMemoryManagementUnit& mmu_;
    UniquePointer<MyPageTable> page_table_;
    Entry entries_[COUNT]; // AFOX_TODO: Implement something like std::array.
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_GENERIC_PAGE_MAP_LEVEL_ONE_CONTROLLER_HPP
