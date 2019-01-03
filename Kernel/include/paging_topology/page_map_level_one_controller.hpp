#ifndef FLAPJACKOS_KERNEL_INCLUDE_PAGING_TOPOLOGY_PAGE_MAP_LEVEL_ONE_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PAGING_TOPOLOGY_PAGE_MAP_LEVEL_ONE_CONTROLLER_HPP

#include <paging_topology/page_frame_controller.hpp>
#include <common/shared_pointer.hpp>

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
    };

    virtual ~PageMapLevelOneController() = default;

    // Returns the number of entries in the table.
    virtual size_t get_number_of_entries() const = 0;

    // Gets the specified entry in the table.
    virtual Entry& get_entry(size_t index) = 0;
    virtual const Entry& get_entry(size_t index) const = 0;
};

} // namespace PagingTopology

#endif // FLAPJACKOS_KERNEL_INCLUDE_PAGING_TOPOLOGY_PAGE_MAP_LEVEL_ONE_CONTROLLER_HPP
