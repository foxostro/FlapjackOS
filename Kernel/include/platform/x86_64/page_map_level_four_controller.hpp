#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PAGE_MAP_LEVEL_FOUR_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PAGE_MAP_LEVEL_FOUR_CONTROLLER_HPP

#include <paging_topology/page_map_level_four_controller.hpp>
#include <platform/x86_64/page_map_level_three_controller.hpp>
#include <platform/x86_64/page_map_level_four.hpp>
#include <common/unique_pointer.hpp>
#include <common/interrupt_lock.hpp>
#include <cassert>

namespace x86_64 {

// Owns a fourth level page map object. Provides synchronized access.
class PageMapLevelFourController final : public PagingTopology::PageMapLevelFourController {
public:
    using ConcretePageMapLevelThreeController = x86_64::PageMapLevelThreeController;
    using MyPageDirectoryPointerTable = x86_64::PageDirectoryPointerTable;
    using MyPageMapLevelFour = x86_64::PageMapLevelFour;
    using MyPageMapLevelFourEntry = typename std::remove_reference<decltype(MyPageMapLevelFour::entries[0])>::type;
    static constexpr size_t COUNT = MyPageMapLevelFour::COUNT;
    static constexpr size_t SIZE_GOVERENED_BY_ENTRY = 0x8000000000; // 512GB

    // A single entry in the page map.
    // This controls the contents of one underlying Page Directory Entry and
    // also owns the page table which is referenced therein.
    class Entry final : public PagingTopology::PageMapLevelFourController::Entry {
    public:
        virtual ~Entry() = default;

        Entry() : lock_(nullptr), pde_(nullptr), mmu_(nullptr) {}

        void set_lock(InterruptLock* lock)
        {
            assert(lock);
            lock_ = lock;
        }

        void set_pde(MyPageMapLevelFourEntry* pde)
        {
            assert(pde);
            pde_ = pde;
        }

        void set_mmu(HardwareMemoryManagementUnit* mmu)
        {
            assert(mmu);
            mmu_ = mmu;
        }

        SharedPointer<PagingTopology::PageMapLevelThreeController> get_pml3() const override
        {
            assert(lock_);
            LockGuard guard{*lock_};
            return pml3_;
        }
        
        void set_pml3(SharedPointer<PagingTopology::PageMapLevelThreeController> p) override
        {
            assert(lock_);
            LockGuard guard{*lock_};
            set_pml3_unlocked(p);
        }
        
        void set_pml3_unlocked(SharedPointer<PagingTopology::PageMapLevelThreeController> p)
        {
            assert(pde_);
            pml3_ = std::move(p);
            if (has_page_table_unlocked()) {
                pde_->set_address(pml3_->get_underlying_object_physical_address());
                pde_->set_present(true);
            } else {
                pde_->set_address(0);
                pde_->set_present(false);
            }
        }

        bool has_page_table_unlocked() const
        {
            return pml3_ && (pml3_->get_underlying_object_physical_address() != 0);
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
        
        void populate(uintptr_t linear_address) override
        {
            assert(lock_);
            LockGuard guard{*lock_};
            if (!pml3_) {
                set_pml3_unlocked(create_pml3());
                pml3_->populate(linear_address);
            }
        }

        UniquePointer<PagingTopology::PageMapLevelThreeController> create_pml3()
        {
            assert(mmu_);
            MyPageDirectoryPointerTable* object = new MyPageDirectoryPointerTable;
            memset(object, 0, sizeof(*object));
            return new ConcretePageMapLevelThreeController{*mmu_, object};
        }

    private:
        InterruptLock* lock_;
        MyPageMapLevelFourEntry* pde_;
        HardwareMemoryManagementUnit* mmu_;
        SharedPointer<PagingTopology::PageMapLevelThreeController> pml3_;
    };

    PageMapLevelFourController(HardwareMemoryManagementUnit& mmu)
     : mmu_(mmu),
       page_directory_(new MyPageMapLevelFour)
    {
        clear_directory();
        feed();
    }

    PageMapLevelFourController(HardwareMemoryManagementUnit& mmu,
                               UniquePointer<MyPageMapLevelFour> object)
     : mmu_(mmu),
       page_directory_(std::move(object))
    {
        feed();
    }

    PageMapLevelFourController(const PageMapLevelFourController&) = delete;
    PageMapLevelFourController(PageMapLevelFourController&&) = delete;

    void clear_directory()
    {
        assert(page_directory_);
        memset(page_directory_.get_pointer(), 0, sizeof(MyPageMapLevelFour));
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
        return SIZE_GOVERENED_BY_ENTRY;
    }

    // Gets the linear address of the underlying paging object.
    void* get_underlying_object_pointer() const override
    {
        LockGuard guard{lock_};
        return page_directory_.get_pointer();
    }

    // Gets the physical address of the underlying paging object.
    uintptr_t get_underlying_object_physical_address() const override
    {
        return mmu_.convert_logical_to_physical_address(reinterpret_cast<uintptr_t>(get_underlying_object_pointer()));
    }

    // Ensures the underlying paging objects have been populated.
    void populate(uintptr_t linear_address) override
    {
        get_pml4_entry_by_address(linear_address).populate(linear_address);
    }
    
    size_t get_index_of_pml4_entry_by_address(uintptr_t linear_address) const override
    {
        constexpr unsigned PML4E_INDEX_SHIFT = 39;
        constexpr unsigned PML4E_INDEX_MASK = 0b111111111; // 9 bits
        size_t index = (linear_address >> PML4E_INDEX_SHIFT) & PML4E_INDEX_MASK;
        return index;
    }

    size_t get_index_of_pml3_entry_by_address(uintptr_t linear_address) const override
    {
        constexpr unsigned PDPTE_INDEX_SHIFT = 30;
        constexpr unsigned PDPTE_INDEX_MASK = 0b111111111; // 9 bits
        size_t index = (linear_address >> PDPTE_INDEX_SHIFT) & PDPTE_INDEX_MASK;
        return index;
    }

    size_t get_index_of_pml2_entry_by_address(uintptr_t linear_address) const override
    {
        constexpr unsigned PDE_INDEX_SHIFT = 21;
        constexpr unsigned PDE_INDEX_MASK = 0b111111111; // 9 bits
        size_t index = (linear_address >> PDE_INDEX_SHIFT) & PDE_INDEX_MASK;
        return index;
    }

    size_t get_index_of_pml1_entry_by_address(uintptr_t linear_address) const override
    {
        constexpr unsigned PTE_INDEX_SHIFT = 12;
        constexpr unsigned PTE_INDEX_MASK = 0b111111111; // 9 bits
        size_t index = (linear_address >> PTE_INDEX_SHIFT) & PTE_INDEX_MASK;
        return index;
    }
    
    PagingTopology::PageMapLevelFourController::Entry& get_pml4_entry_by_address(uintptr_t linear_address) override
    {
        return get_entry(get_index_of_pml4_entry_by_address(linear_address));
    }
    
    PagingTopology::PageMapLevelThreeController::Entry& get_pml3_entry_by_address(uintptr_t linear_address) override
    {
        auto pml3 = get_pml3(linear_address);
        assert(pml3);
        return pml3->get_pml3_entry_by_address(linear_address);
    }

    PagingTopology::PageMapLevelTwoController::Entry& get_pml2_entry_by_address(uintptr_t linear_address) override
    {
        return get_pml2(linear_address)->get_pml2_entry_by_address(linear_address);
    }

    PagingTopology::PageMapLevelOneController::Entry& get_pml1_entry_by_address(uintptr_t linear_address) override
    {
        return get_pml1(linear_address)->get_pml1_entry_by_address(linear_address);
    }

    SharedPointer<PagingTopology::PageMapLevelThreeController> get_pml3(uintptr_t linear_address) override
    {
        return get_entry(get_index_of_pml4_entry_by_address(linear_address)).get_pml3();
    }

    SharedPointer<PagingTopology::PageMapLevelTwoController> get_pml2(uintptr_t linear_address) override
    {
        return get_pml3(linear_address)->get_pml3_entry_by_address(linear_address).get_pml2();
    }

    SharedPointer<PagingTopology::PageMapLevelOneController> get_pml1(uintptr_t linear_address) override
    {
        return get_pml2(linear_address)->get_pml2_entry_by_address(linear_address).get_pml1();
    }

private:
    mutable InterruptLock lock_;
    HardwareMemoryManagementUnit& mmu_;
    UniquePointer<MyPageMapLevelFour> page_directory_;
    Entry entries_[COUNT]; // AFOX_TODO: Implement something like std::array.
};

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PAGE_MAP_LEVEL_FOUR_CONTROLLER_HPP
