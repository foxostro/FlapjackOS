#ifndef FLAPJACKOS_KERNEL_INCLUDE_PAGING_TOPOLOGY_PAGE_MAP_LEVEL_FOUR_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PAGING_TOPOLOGY_PAGE_MAP_LEVEL_FOUR_CONTROLLER_HPP

#include <paging_topology/page_map_level_three_controller.hpp>

namespace PagingTopology {

// Owns a fourth level page map.
// For example, on IA-32e this object is called a PML4.
class PageMapLevelFourController {
public:
    // A single entry in the page map.
    class Entry {
    public:
        virtual ~Entry() = default;

        virtual SharedPointer<PageMapLevelThreeController> get_pml3() const = 0;
        virtual void set_pml3(SharedPointer<PageMapLevelThreeController> p) = 0;

        virtual bool is_present() const = 0;
        virtual void set_present(bool present) = 0;

        virtual bool is_readwrite() const = 0;
        virtual void set_readwrite(bool readwrite) = 0;

        virtual bool is_supervisor() const = 0;
        virtual void set_supervisor(bool supervisor) = 0;

        virtual bool is_accessed() const = 0;
        virtual void set_accessed(bool accessed) = 0;

        virtual void set_protection(ProtectionFlags flags) = 0;

        virtual void populate(uintptr_t linear_address) = 0;
    };

    virtual ~PageMapLevelFourController() = default;

    // Returns the number of entries in the table.
    virtual size_t get_number_of_entries() const = 0;

    // Gets the specified entry in the table.
    virtual Entry& get_entry(size_t index) = 0;
    virtual const Entry& get_entry(size_t index) const = 0;

    // Gets the size of the region of memory governed by each entry.
    virtual size_t get_size_governed_by_entry() const = 0;

    // Gets the linear address of the underlying physical paging object.
    virtual void* get_underlying_object_pointer() const = 0;

    // Gets the physical address of the underlying page directory object.
    virtual uintptr_t get_underlying_object_physical_address() const = 0;

    // Ensures the underlying paging objects have been populated.
    virtual void populate(uintptr_t linear_address) = 0;

    virtual size_t get_index_of_pml4_entry_by_address(uintptr_t linear_address) const = 0;
    virtual size_t get_index_of_pml3_entry_by_address(uintptr_t linear_address) const = 0;
    virtual size_t get_index_of_pml2_entry_by_address(uintptr_t linear_address) const = 0;
    virtual size_t get_index_of_pml1_entry_by_address(uintptr_t linear_address) const = 0;
    
    virtual Entry& get_pml4_entry_by_address(uintptr_t linear_address) = 0;
    virtual PageMapLevelThreeController::Entry& get_pml3_entry_by_address(uintptr_t linear_address) = 0;
    virtual PageMapLevelTwoController::Entry& get_pml2_entry_by_address(uintptr_t linear_address) = 0;
    virtual PageMapLevelOneController::Entry& get_pml1_entry_by_address(uintptr_t linear_address) = 0;
    
    virtual SharedPointer<PageMapLevelThreeController> get_pml3(uintptr_t linear_address) = 0;
    virtual SharedPointer<PageMapLevelTwoController> get_pml2(uintptr_t linear_address) = 0;
    virtual SharedPointer<PageMapLevelOneController> get_pml1(uintptr_t linear_address) = 0;
};

} // namespace PagingTopology

#endif // FLAPJACKOS_KERNEL_INCLUDE_PAGING_TOPOLOGY_PAGE_MAP_LEVEL_FOUR_CONTROLLER_HPP
