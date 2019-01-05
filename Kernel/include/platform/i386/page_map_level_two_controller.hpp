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

        Entry() : lock_(nullptr), pde_(nullptr) {}

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

        SharedPointer<PagingTopology::PageMapLevelOneController> get_pml1() const override
        {
            assert(lock_);
            LockGuard guard{*lock_};
            return pml1_;
        }
        
        void set_pml1(SharedPointer<PagingTopology::PageMapLevelOneController> p) override
        {
            assert(pde_);
            assert(lock_);
            LockGuard guard{*lock_};
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

    private:
        Mutex* lock_;
        PageDirectoryEntry* pde_;
        SharedPointer<PagingTopology::PageMapLevelOneController> pml1_;
    };

    static constexpr size_t COUNT = PageDirectory::COUNT;

    PageMapLevelTwoController()
     : PageMapLevelTwoController(new PageDirectory)
    {}

    PageMapLevelTwoController(UniquePointer<PageDirectory> pd)
     : page_directory_(std::move(pd))
    {
        clear_directory();
        feed_pdes();
        feed_lock();
    }

    PageMapLevelTwoController(const PageMapLevelTwoController&) = delete;
    PageMapLevelTwoController(PageMapLevelTwoController&&) = delete;

    void clear_directory()
    {
        assert(page_directory_);
        memset(page_directory_.get_pointer(), 0, sizeof(PageDirectory));
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

private:
    mutable Mutex lock_;
    UniquePointer<PageDirectory> page_directory_;
    Entry entries_[COUNT]; // AFOX_TODO: Implement something like std::array.
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_MAP_LEVEL_TWO_CONTROLLER_HPP
