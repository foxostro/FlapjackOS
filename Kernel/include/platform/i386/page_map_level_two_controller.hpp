#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_MAP_LEVEL_TWO_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_MAP_LEVEL_TWO_CONTROLLER_HPP

#include <platform/i386/page_directory.hpp>
#include <paging_topology/page_map_level_two_controller.hpp>
#include <common/unique_pointer.hpp>
#include <common/mutex.hpp>
#include <cassert>

namespace i386 {

// Owns a page directory. Provides synchronized access.
// On IA-32, the second level page map is called a Page Directory.
class PageMapLevelTwoController final : public PagingTopology::PageMapLevelTwoController {
public:
    // A single entry in the page map.
    // This controls the contents of one underlying Page Directory Entry and
    // also owns the page table which is referenced therein.
    class Entry final : public PagingTopology::PageMapLevelTwoController::Entry {
    public:
        virtual ~Entry() = default;

        Entry() : lock_(nullptr), pde_(nullptr), mmu_(nullptr) {}

        void set_lock(Mutex* lock)
        {
            assert(lock);
            lock_ = lock;
        }

        void set_pde(PageDirectoryEntry* pde)
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
                pde_->set_address(pml1_->get_page_table_physical_address());
                pde_->set_present(true);
            } else {
                pde_->set_address(0);
                pde_->set_present(false);
            }
        }

        bool has_page_table_unlocked() const
        {
            return pml1_ && (pml1_->get_page_table_physical_address() != 0);
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
        
        void populate() override
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
            i386::PageTable* page_table = new i386::PageTable;
            memset(page_table, 0, sizeof(*page_table));
            return new i386::PageMapLevelOneController{*mmu_, page_table};
        }

    private:
        Mutex* lock_;
        PageDirectoryEntry* pde_;
        HardwareMemoryManagementUnit* mmu_;
        SharedPointer<PagingTopology::PageMapLevelOneController> pml1_;
    };

    static constexpr size_t COUNT = PageDirectory::COUNT;

    PageMapLevelTwoController(HardwareMemoryManagementUnit& mmu)
     : mmu_(mmu),
       page_directory_(new PageDirectory)
    {
        clear_directory();
        feed();
    }

    PageMapLevelTwoController(HardwareMemoryManagementUnit& mmu,
                              UniquePointer<PageDirectory> pd)
     : mmu_(mmu),
       page_directory_(std::move(pd))
    {
        feed();
    }

    PageMapLevelTwoController(const PageMapLevelTwoController&) = delete;
    PageMapLevelTwoController(PageMapLevelTwoController&&) = delete;

    void clear_directory()
    {
        assert(page_directory_);
        memset(page_directory_.get_pointer(), 0, sizeof(PageDirectory));
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
        constexpr uintptr_t FOUR_MEGS = 0x400000;
        return FOUR_MEGS;
    }

    // Indicates that the underlying page table object be allowed to leak.
    // This is useful when the object was not allocated on the heap.
    void set_should_leak() override
    {
        LockGuard guard{lock_};
        page_directory_.set_should_leak();
    }

    // Gets the linear address of the underlying page directory object.
    void* get_page_directory_pointer() const override
    {
        LockGuard guard{lock_};
        return page_directory_.get_pointer();
    }

    // Ensures the underlying paging objects have been populated for the
    // specified offset into the PML2. This allocates memory for the
    // corresponding PML1 object.
    void populate(uintptr_t offset) override
    {
        get_entry_by_offset(offset).populate();
    }

private:
    mutable Mutex lock_;
    HardwareMemoryManagementUnit& mmu_;
    UniquePointer<PageDirectory> page_directory_;
    Entry entries_[COUNT]; // AFOX_TODO: Implement something like std::array.
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_MAP_LEVEL_TWO_CONTROLLER_HPP
