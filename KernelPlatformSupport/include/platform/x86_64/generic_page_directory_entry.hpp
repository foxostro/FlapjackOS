#ifndef FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_GENERIC_PAGE_DIRECTORY_ENTRY_HPP
#define FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_GENERIC_PAGE_DIRECTORY_ENTRY_HPP

#include <cstdint>

namespace x86_64 {

// Generic entry in a paging table under x86_64.
//
// According to the Intel manual, volume 3a, section 4.5, several of the paging
// structures have entries that follow an identical format. That format is
// reproduced here, in this generic structure.
//
// This is suitable for use in a Page Map Level Four, a Page Directory Pointer
// Table, and a Page Directory structure. This is never acceptable for use in a
// Page Table. Documented layout in memory is as follows:
//
// ---------------------------------------------------------------------------------
// | 63 | 62:52 | 51:M | (M-1):12  | 11:8 |  7 | 6 | 5 |  4  |  3  |  2  |  1  | 0 |
// |-------------------------------------------------------------------------------|
// | XD |     x |    0 | phys addr |    x | PS | x | A | PCD | PWT | U/S | R/W | P |
// ---------------------------------------------------------------------------------
//
// Where...
// x  -- Ignored
// 0  -- Must be zero
// XD -- Execute Disable
// phys addr -- The physical address of the paging structure to which this entry
//              refers, e.g., a page table.
// PS -- Page Size, which must be zero else we'd use a different object.
// A -- Accessed
// PCD -- Page-level Cache Disable
// PWT -- Page-level Write-Through
// U/S -- User / Supervisor
// R/W -- Read-only / Read-Write
// P -- Present
class GenericPageDirectoryEntry {
public:
    static constexpr uint64_t PRESENT         = 1 << 0;
    static constexpr uint64_t READWRITE       = 1 << 1;
    static constexpr uint64_t SUPERVISOR      = 1 << 2;
    static constexpr uint64_t PWT             = 1 << 3;
    static constexpr uint64_t PCD             = 1 << 4;
    static constexpr uint64_t ACCESSED        = 1 << 5;
    static constexpr uint64_t EXECUTE_DISABLE = 1ull << 63;
    static constexpr uint64_t PAGE_SIZE_SHIFT = 7;
    static constexpr uint64_t PAGE_SIZE_MASK  = 1 << PAGE_SIZE_SHIFT;
    static constexpr uint64_t ADDRESS_ZERO_MASK = 0b1111111111110000000000000000000000000000000000000000111111111111;
    static constexpr uint64_t ADDRESS_MASK = 0b1111111111111111111111111111111111111111;
    static constexpr uint64_t ADDRESS_SHIFT = 12;

    uint64_t data;

    GenericPageDirectoryEntry() = default;

    bool is_present() const
    {
        return !!(data & PRESENT);
    }

    void set_present(bool present)
    {
        if (present) {
            data = data | PRESENT;
        } else {
            data = data & ~PRESENT;
        }
    }

    bool is_readwrite() const
    {
        return !!(data & READWRITE);
    }

    void set_readwrite(bool readwrite)
    {
        if (readwrite) {
            data = data | READWRITE;
        } else {
            data = data & ~READWRITE;
        }
    }

    bool is_supervisor() const
    {
        return !!(data & SUPERVISOR);
    }

    void set_supervisor(bool supervisor)
    {
        if (supervisor) {
            data = data | SUPERVISOR;
        } else {
            data = data & ~SUPERVISOR;
        }
    }

    bool is_page_write_through() const
    {
        return !!(data & PWT);
    }

    void set_page_write_through(bool enable)
    {
        if (enable) {
            data = data | PWT;
        } else {
            data = data & ~PWT;
        }
    }

    bool is_page_cache_disabled() const
    {
        return !!(data & PCD);
    }

    void set_page_cache_disabled(bool disabled)
    {
        if (disabled) {
            data = data | PCD;
        } else {
            data = data & ~PCD;
        }
    }

    bool is_accessed() const
    {
        return !!(data & ACCESSED);
    }

    void set_accessed(bool accessed)
    {
        if (accessed) {
            data = data | ACCESSED;
        } else {
            data = data & ~ACCESSED;
        }
    }

    int get_page_size() const
    {
        return (data & PAGE_SIZE_MASK) >> PAGE_SIZE_SHIFT;
    }

    bool is_execute_disabled() const
    {
        return !!(data & EXECUTE_DISABLE);
    }

    void set_execute_disabled(bool disabled)
    {
        if (disabled) {
            data = data | EXECUTE_DISABLE;
        } else {
            data = data & ~EXECUTE_DISABLE;
        }
    }

    void set_address(uintptr_t address)
    {
        uint64_t masked_address = ((address >> ADDRESS_SHIFT) & ADDRESS_MASK);
        uint64_t field = masked_address << ADDRESS_SHIFT;
        data = (data & ADDRESS_ZERO_MASK) | field;
    }

    uint64_t get_address() const
    {
        return (data & ~ADDRESS_ZERO_MASK);
    }
};

} // namespace x86_64

#endif // FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_GENERIC_PAGE_DIRECTORY_ENTRY_HPP
