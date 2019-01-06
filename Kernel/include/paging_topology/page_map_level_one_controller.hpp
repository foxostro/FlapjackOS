#ifndef FLAPJACKOS_KERNEL_INCLUDE_PAGING_TOPOLOGY_PAGE_MAP_LEVEL_ONE_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PAGING_TOPOLOGY_PAGE_MAP_LEVEL_ONE_CONTROLLER_HPP

#include <paging_topology/page_frame_controller.hpp>
#include <common/shared_pointer.hpp>
#include <protection_flags.hpp>

namespace PagingTopology {

// Owns a first level page map.
// For example, on IA-32 and IA-32e this object is called a Page Table.
class PageMapLevelOneController {
public:
    // A single entry in the page map.
    class Entry {
    public:
        virtual ~Entry() = default;

        virtual SharedPointer<PageFrameController> get_page_frame() const = 0;
        virtual void set_page_frame(SharedPointer<PageFrameController> p) = 0;

        virtual bool is_present() const = 0;
        virtual void set_present(bool present) = 0;

        virtual bool is_readwrite() const = 0;
        virtual void set_readwrite(bool readwrite) = 0;

        virtual bool is_supervisor() const = 0;
        virtual void set_supervisor(bool supervisor) = 0;

        virtual bool is_accessed() const = 0;
        virtual void set_accessed(bool accessed) = 0;

        virtual bool is_dirty() const = 0;
        virtual void set_dirty(bool dirty) = 0;

        virtual bool is_global() const = 0;
        virtual void set_global(bool global) = 0;

        virtual void set_protection(ProtectionFlags flags) = 0;

        virtual void set_mapping(uintptr_t physical_address,
                                 ProtectionFlags flags) = 0;

        virtual UniquePointer<PagingTopology::PageFrameController> create_page_frame_controller(uintptr_t page_frame) = 0;
    };

    virtual ~PageMapLevelOneController() = default;

    // Gets the physical address of the underlying page table.
    virtual uintptr_t get_underlying_object_physical_address() const = 0;

    // Returns the number of entries in the table.
    virtual size_t get_number_of_entries() const = 0;

    // Gets the specified entry in the table.
    virtual Entry& get_entry(size_t index) = 0;
    virtual const Entry& get_entry(size_t index) const = 0;

    // Gets the size of the region of memory governed by each entry.
    virtual size_t get_size_governed_by_entry() const = 0;

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
};

} // namespace PagingTopology

#endif // FLAPJACKOS_KERNEL_INCLUDE_PAGING_TOPOLOGY_PAGE_MAP_LEVEL_ONE_CONTROLLER_HPP
