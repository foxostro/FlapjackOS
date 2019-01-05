#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_MAP_LEVEL_ONE_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_MAP_LEVEL_ONE_CONTROLLER_HPP

#include <platform/i386/page_table.hpp>
#include <paging_topology/page_map_level_one_controller.hpp>
#include <hardware_memory_management_unit.hpp>
#include <common/unique_pointer.hpp>
#include <common/mutex.hpp>
#include <cassert>

namespace i386 {

// Owns a page table. Provides synchronized access.
// On IA-32, the first level page map is called a Page Table.
class PageMapLevelOneController final : public PagingTopology::PageMapLevelOneController {
public:
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

        void set_pte(PageTableEntry* pte)
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
            assert(pte_);
            assert(lock_);
            LockGuard guard{*lock_};
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

    private:
        Mutex* lock_;
        PageTableEntry* pte_;
        SharedPointer<PagingTopology::PageFrameController> page_frame_;
    };

    static constexpr size_t COUNT = PageTable::COUNT;

    PageMapLevelOneController(HardwareMemoryManagementUnit& mmu)
     : PageMapLevelOneController(mmu, new PageTable)
    {
        clear_table();
        feed_ptes();
        feed_lock();
    }

    PageMapLevelOneController(HardwareMemoryManagementUnit& mmu,
                              UniquePointer<PageTable> pt)
     : mmu_(mmu),
       page_table_(std::move(pt))
    {
        feed_ptes();
        feed_lock();
    }

    PageMapLevelOneController(const PageMapLevelOneController&) = delete;
    PageMapLevelOneController(PageMapLevelOneController&&) = delete;

    void clear_table()
    {
        assert(page_table_);
        memset(page_table_.get_pointer(), 0, sizeof(PageTable));
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
    uintptr_t get_page_table_physical_address() const override
    {
        LockGuard guard{lock_};
        PageTable* page_table = page_table_.get_pointer();
        uintptr_t logical = reinterpret_cast<uintptr_t>(page_table);
        uintptr_t physical = mmu_.convert_logical_to_physical_address(logical);
        return physical;
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

    // Indicates that the underlying page table object be allowed to leak.
    // This is useful when the object was not allocated on the heap.
    void set_should_leak() override
    {
        LockGuard guard{lock_};
        page_table_.set_should_leak();
    }

private:
    mutable Mutex lock_;
    HardwareMemoryManagementUnit& mmu_;
    UniquePointer<PageTable> page_table_;
    Entry entries_[COUNT]; // AFOX_TODO: Implement something like std::array.
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_MAP_LEVEL_ONE_CONTROLLER_HPP
