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

        virtual void populate(uintptr_t offset) = 0;
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
    virtual void* get_physical_object_pointer() const = 0;

    // Gets the physical address of the underlying page directory object.
    virtual uintptr_t get_underlying_object_physical_address() const = 0;

    // Ensures the underlying paging objects have been populated for the
    // specified offset into the PML4. This allocates memory for the
    // corresponding PML3 object.
    virtual void populate(uintptr_t offset) = 0;

    // Gets the PML3 entry associated with the specified offset.
    // The PML4 governs a region of memory and the offset is an offset from
    // the beginning of that region of memory. Each entry into the PML4, each
    // PML3, and each entry into the PML3 are associated with ranges within
    // this region of memory. All can be located with an offset into the PML4.
    // AFOX_TODO: Implement an Optional class so I can return "none" when intermediate paging objects are missing.
    PageMapLevelThreeController::Entry& get_pml3_entry_by_offset(uintptr_t offset)
    {
        auto& pml4_entry = get_entry_by_offset(offset);
        auto pml3 = pml4_entry.get_pml3();
        assert(pml3 && "The PML3 is missing and there is no associated PML3 entry. (The page map is sparse.)");
        return pml3->get_entry_by_offset(get_corresponding_pml3_offset(offset));
    }

    // Gets an entry associated with the specified offset into the page map.
    // The page map governs a region of memory and the offset is an offset from
    // the beginning of that region of memory. Each entry is associated with a
    // range of addresses within that region.
    Entry& get_entry_by_offset(uintptr_t offset)
    {
        return get_entry(get_index_of_entry_by_offset(offset));
    }

    // Gets the index of an entry associated with the specified offset.
    // The page map governs a region of memory and the offset is an offset from
    // the beginning of that region of memory. Each entry is associated with a
    // range of addresses within that region.
    size_t get_index_of_entry_by_offset(uintptr_t offset) const
    {
        size_t index = offset / get_size_governed_by_entry();
        assert(index < get_number_of_entries());
        return index;
    }
    
    // Gets the corresponding offset into the corresponding PML3.
    // Each offset into the region of memory governed by the PML4 is associated
    // both with a specific PML3 and also with an offset into the region of
    // memory governed by that PML3.
    uintptr_t get_corresponding_pml3_offset(uintptr_t pml4_offset)
    {
        size_t pml4_index = get_index_of_entry_by_offset(pml4_offset);
        size_t pml4_entry_size = get_size_governed_by_entry();
        uintptr_t pml3_offset = (pml4_offset - (pml4_index * pml4_entry_size));
        return pml3_offset;
    }
};

} // namespace PagingTopology

#endif // FLAPJACKOS_KERNEL_INCLUDE_PAGING_TOPOLOGY_PAGE_MAP_LEVEL_FOUR_CONTROLLER_HPP
