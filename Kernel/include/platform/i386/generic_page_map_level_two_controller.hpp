#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_GENERIC_PAGE_MAP_LEVEL_TWO_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_GENERIC_PAGE_MAP_LEVEL_TWO_CONTROLLER_HPP

#include <paging_topology/page_map_level_two_controller.hpp>
#include <common/unique_pointer.hpp>
#include <common/interrupt_lock.hpp>
#include <cassert>

namespace i386 {

// Owns a page directory. Provides synchronized access.
// On IA-32, the second level page map is called a Page Directory.
template<typename Policy>
class GenericPageMapLevelTwoController : public PagingTopology::PageMapLevelTwoController {
public:
    using ConcretePageMapLevelOneController = typename Policy::ConcretePageMapLevelOneController;
    using MyPageTable = typename Policy::MyPageTable;
    using MyPageDirectory = typename Policy::MyPageDirectory;
    using MyPageDirectoryEntry = typename std::remove_reference<decltype(MyPageDirectory::entries[0])>::type;
    static constexpr size_t COUNT = MyPageDirectory::COUNT;

    // A single entry in the page map.
    // This controls the contents of one underlying Page Directory Entry and
    // also owns the page table which is referenced therein.
    class Entry final : public PagingTopology::PageMapLevelTwoController::Entry {
    public:
        virtual ~Entry() = default;

        Entry() : lock_(nullptr), pde_(nullptr), mmu_(nullptr) {}

        void set_lock(InterruptLock* lock)
        {
            assert(lock);
            lock_ = lock;
        }

        void set_pde(MyPageDirectoryEntry* pde)
        {
            assert(pde);
            pde_ = pde;
        }

        void set_mmu(HardwareMemoryManagementUnit* mmu)
        {
            assert(mmu);
            mmu_ = mmu;
        }

        SharedPointer<PagingTopology::PageMapLevelOneController> get_pml1() const override
        {
            assert(lock_);
            LockGuard guard{*lock_};
            return pml1_;
        }
        
        void set_pml1(SharedPointer<PagingTopology::PageMapLevelOneController> p) override
        {
            assert(lock_);
            LockGuard guard{*lock_};
            set_pml1_unlocked(p);
        }
        
        void set_pml1_unlocked(SharedPointer<PagingTopology::PageMapLevelOneController> p)
        {
            assert(pde_);
            pml1_ = std::move(p);
            if (has_page_table_unlocked()) {
                pde_->set_address(pml1_->get_underlying_object_physical_address());
                pde_->set_present(true);
            } else {
                pde_->set_address(0);
                pde_->set_present(false);
            }
        }

        bool has_page_table_unlocked() const
        {
            return pml1_ && (pml1_->get_underlying_object_physical_address() != 0);
        }
        
        bool is_present() const override
        {
            assert(pde_);
            assert(lock_);
            LockGuard guard{*lock_};
            return pde_->is_present();
        }

        void set_present(bool present) override
        {
            assert(pde_);
            assert(lock_);
            LockGuard guard{*lock_};
            pde_->set_present(present);
        }

        bool is_readwrite() const override
        {
            assert(pde_);
            assert(lock_);
            LockGuard guard{*lock_};
            return pde_->is_readwrite();
        }

        void set_readwrite(bool readwrite) override
        {
            assert(pde_);
            assert(lock_);
            LockGuard guard{*lock_};
            pde_->set_readwrite(readwrite);
        }

        bool is_supervisor() const override
        {
            assert(pde_);
            assert(lock_);
            LockGuard guard{*lock_};
            return pde_->is_supervisor();
        }

        void set_supervisor(bool supervisor) override
        {
            assert(pde_);
            assert(lock_);
            LockGuard guard{*lock_};
            pde_->set_supervisor(supervisor);
        }

        bool is_accessed() const override
        {
            assert(pde_);
            assert(lock_);
            LockGuard guard{*lock_};
            return pde_->is_accessed();
        }

        void set_accessed(bool accessed) override
        {
            assert(pde_);
            assert(lock_);
            LockGuard guard{*lock_};
            pde_->set_accessed(accessed);
        }

        void set_protection(ProtectionFlags flags) override
        {
            assert(pde_);
            assert(lock_);
            LockGuard guard{*lock_};
            pde_->set_readwrite((flags & WRITABLE) != 0);
            pde_->set_supervisor((flags & SUPERVISOR) != 0);
        }
        
        void populate([[maybe_unused]] uintptr_t linear_address) override
        {
            assert(lock_);
            LockGuard guard{*lock_};
            if (!pml1_) {
                set_pml1_unlocked(create_pml1());
            }
        }

        UniquePointer<PagingTopology::PageMapLevelOneController> create_pml1()
        {
            assert(mmu_);
            MyPageTable* page_table = new MyPageTable;
            memset(page_table, 0, sizeof(*page_table));
            return new ConcretePageMapLevelOneController{*mmu_, page_table};
        }

    private:
        InterruptLock* lock_;
        MyPageDirectoryEntry* pde_;
        HardwareMemoryManagementUnit* mmu_;
        SharedPointer<PagingTopology::PageMapLevelOneController> pml1_;
    };

    GenericPageMapLevelTwoController(HardwareMemoryManagementUnit& mmu)
     : mmu_(mmu),
       page_directory_(new MyPageDirectory)
    {
        clear_directory();
        feed();
    }

    GenericPageMapLevelTwoController(HardwareMemoryManagementUnit& mmu,
                                     UniquePointer<MyPageDirectory> pd)
     : mmu_(mmu),
       page_directory_(std::move(pd))
    {
        feed();
    }

    GenericPageMapLevelTwoController(const GenericPageMapLevelTwoController&) = delete;
    GenericPageMapLevelTwoController(GenericPageMapLevelTwoController&&) = delete;

    void clear_directory()
    {
        assert(page_directory_);
        memset(page_directory_.get_pointer(), 0, sizeof(MyPageDirectory));
    }

    void feed()
    {
        feed_pdes();
        feed_lock();
        feed_mmu();
    }

    void feed_pdes()
    {
        assert(page_directory_);
        for (size_t i = 0; i < COUNT; ++i) {
            auto& pde = page_directory_->entries[i];
            entries_[i].set_pde(&pde);
        }
    }

    void feed_lock()
    {
        for (size_t i = 0; i < COUNT; ++i) {
            entries_[i].set_lock(&lock_);
        }
    }

    void feed_mmu()
    {
        for (size_t i = 0; i < COUNT; ++i) {
            entries_[i].set_mmu(&mmu_);
        }
    }

    // Returns the number of entries in the table.
    size_t get_number_of_entries() const override
    {
        return COUNT;
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
        return Policy::SIZE_GOVERENED_BY_ENTRY;
    }

    // Gets the linear address of the underlying page directory object.
    // AFOX_TODO: rename to get_underlying_object_pointer() or something
    void* get_page_directory_pointer() const override
    {
        LockGuard guard{lock_};
        return page_directory_.get_pointer();
    }

    // Gets the physical address of the underlying page directory object.
    uintptr_t get_underlying_object_physical_address() const override
    {
        return mmu_.convert_logical_to_physical_address(reinterpret_cast<uintptr_t>(get_page_directory_pointer()));
    }

    // Ensures the underlying paging objects have been populated.
    void populate(uintptr_t linear_address) override
    {
        get_pml2_entry_by_address(linear_address).populate(linear_address);
    }

    PagingTopology::PageMapLevelTwoController::Entry& get_pml2_entry_by_address(uintptr_t linear_address) override
    {
        return get_entry(get_index_of_pml2_entry_by_address(linear_address));
    }

    PagingTopology::PageMapLevelOneController::Entry& get_pml1_entry_by_address(uintptr_t linear_address) override
    {
        return get_pml1(linear_address)->get_pml1_entry_by_address(linear_address);
    }

    SharedPointer<PagingTopology::PageMapLevelOneController> get_pml1(uintptr_t linear_address) override
    {
        return get_pml2_entry_by_address(linear_address).get_pml1();
    }

private:
    mutable InterruptLock lock_;
    HardwareMemoryManagementUnit& mmu_;
    UniquePointer<MyPageDirectory> page_directory_;
    Entry entries_[COUNT]; // AFOX_TODO: Implement something like std::array.
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_GENERIC_PAGE_MAP_LEVEL_TWO_CONTROLLER_HPP
