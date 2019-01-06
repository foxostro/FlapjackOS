#ifndef FLAPJACKOS_KERNEL_INCLUDE_PAGING_TOPOLOGY_PAGE_MAP_LEVEL_TWO_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PAGING_TOPOLOGY_PAGE_MAP_LEVEL_TWO_CONTROLLER_HPP

#include <paging_topology/page_map_level_one_controller.hpp>
#include <protection_flags.hpp>

namespace PagingTopology {

// Owns a second level page map.
// For example, on IA-32 and IA-32e this object is called a Page Directory.
class PageMapLevelTwoController {
public:
    // A single entry in the page map.
    class Entry {
    public:
        virtual ~Entry() = default;

        virtual SharedPointer<PageMapLevelOneController> get_pml1() const = 0;
        virtual void set_pml1(SharedPointer<PageMapLevelOneController> p) = 0;

        virtual bool is_present() const = 0;
        virtual void set_present(bool present) = 0;

        virtual bool is_readwrite() const = 0;
        virtual void set_readwrite(bool readwrite) = 0;

        virtual bool is_supervisor() const = 0;
        virtual void set_supervisor(bool supervisor) = 0;

        virtual bool is_accessed() const = 0;
        virtual void set_accessed(bool accessed) = 0;

        virtual void set_protection(ProtectionFlags flags) = 0;

        virtual void populate() = 0;
    };

    virtual ~PageMapLevelTwoController() = default;

    // Returns the number of entries in the table.
    virtual size_t get_number_of_entries() const = 0;

    // Gets the specified entry in the table.
    virtual Entry& get_entry(size_t index) = 0;
    virtual const Entry& get_entry(size_t index) const = 0;

    // Gets the size of the region of memory governed by each entry.
    virtual size_t get_size_governed_by_entry() const = 0;

    // Indicates that the underlying page directory object be allowed to leak.
    // This is useful when the object was not allocated on the heap.
    virtual void set_should_leak() = 0;

    // Gets the linear address of the underlying page directory object.
    virtual void* get_page_directory_pointer() const = 0;

    // Ensures the underlying paging objects have been populated for the
    // specified offset into the PML2. This allocates memory for the
    // corresponding PML1 object.
    virtual void populate(uintptr_t offset) = 0;

    // Gets the PML1 entry associated with the specified offset.
    // The PML2 governs a region of memory and the offset is an offset from
    // the beginning of that region of memory. Each entry into the PML2, each
    // PML1, and each entry into the PML1 are associated with ranges within
    // this region of memory. All can be located with an offset into the PML2.
    // AFOX_TODO: Implement an Optional class so I can return "none" when intermediate paging objects are missing.
    PageMapLevelOneController::Entry& get_pml1_entry_by_offset(uintptr_t offset)
    {
        auto& pml2_entry = get_entry_by_offset(offset);
        auto pml1 = pml2_entry.get_pml1();
        assert(pml1 && "The PML1 is missing and there is no associated PML1 entry. (The page map is sparse.)");
        return pml1->get_entry_by_offset(get_corresponding_pml1_offset(offset));
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
    
    // Gets the corresponding offset into the corresponding PML1.
    // Each offset into the region of memory governed by the PML2 is associated
    // both with a specific PML1 and also with an offset into the region of
    // memory governed by that PML1.
    uintptr_t get_corresponding_pml1_offset(uintptr_t pml2_offset)
    {
        size_t pml2_index = get_index_of_entry_by_offset(pml2_offset);
        size_t pml2_entry_size = get_size_governed_by_entry();
        uintptr_t pml1_offset = (pml2_offset - (pml2_index * pml2_entry_size));
        return pml1_offset;
    }
};

} // namespace PagingTopology

#endif // FLAPJACKOS_KERNEL_INCLUDE_PAGING_TOPOLOGY_PAGE_MAP_LEVEL_TWO_CONTROLLER_HPP
