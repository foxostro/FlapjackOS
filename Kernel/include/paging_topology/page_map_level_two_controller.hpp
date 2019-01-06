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
};

} // namespace PagingTopology

#endif // FLAPJACKOS_KERNEL_INCLUDE_PAGING_TOPOLOGY_PAGE_MAP_LEVEL_TWO_CONTROLLER_HPP
