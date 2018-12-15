#ifndef FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_PAGE_TABLE_ENTRY_HPP
#define FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_PAGE_TABLE_ENTRY_HPP

#include <cstdint>
#include "page_size.hpp"

namespace x86_64 {

// Page table entry under x86_64.
//
// According to the Intel manual, volume 3a, section 4.5, table 4-19, the
// memory layout of a page table entry is as follows:
//
// ----------------------------------------------------------------------------------------------
// | 63 | 62:59 | 58:52 | 51:M | (M-1):12  | 11:9 | 8 |  7  | 6 | 5 |  4  |  3  |  2  |  1  | 0 |
// |--------------------------------------------------------------------------------------------|
// | XD |   key |     x |    0 | phys addr |      | G | PAT | D | A | PCD | PWT | U/S | R/W | P |
// ----------------------------------------------------------------------------------------------
//
// Where...
// x  -- Ignored
// 0  -- Must be zero
// XD -- Execute Disable
// key -- 4-bit protection key, but we won't be using these. (see section 4.6.2)
// phys addr -- The physical address of the 4KB page frame.
// G -- Global
// PAT -- Explained in more detail in section 4.9.2.
// D -- Dirty
// A -- Accessed
// PCD -- Page-level Cache Disable
// PWT -- Page-level Write-Through
// U/S -- User / Supervisor
// R/W -- Read-only / Read-Write
// P -- Present
class PageTableEntry {
public:
    static constexpr uint64_t PRESENT         = 1 << 0;
    static constexpr uint64_t READWRITE       = 1 << 1;
    static constexpr uint64_t SUPERVISOR      = 1 << 2;
    static constexpr uint64_t PWT             = 1 << 3;
    static constexpr uint64_t PCD             = 1 << 4;
    static constexpr uint64_t ACCESSED        = 1 << 5;
    static constexpr uint64_t DIRTY           = 1 << 6;
    static constexpr uint64_t PAT             = 1 << 7;
    static constexpr uint64_t GLOBAL          = 1 << 8;
    static constexpr uint64_t EXECUTE_DISABLE = 1ull << 63;
    static constexpr uint64_t ADDRESS_ZERO_MASK = 0b1111111111110000000000000000000000000000000000000000111111111111;
    static constexpr uint64_t ADDRESS_SIGN_EXT_MASK = 0b1111111111110000000000000000000000000000000000000000000000000000;
    static constexpr uint64_t ADDRESS_HIGH_BIT = 0x8000000000000;

    uint64_t data;

    PageTableEntry() = default;

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

    bool is_dirty() const
    {
        return !!(data & DIRTY);
    }

    void set_dirty(bool dirty)
    {
        if (dirty) {
            data = data | DIRTY;
        } else {
            data = data & ~DIRTY;
        }
    }

    bool is_pat_bit_set() const
    {
        return !!(data & PAT);
    }

    void set_pat_bit(bool pat)
    {
        if (pat) {
            data = data | PAT;
        } else {
            data = data & ~PAT;
        }
    }

    bool is_global() const
    {
        return !!(data & GLOBAL);
    }

    void set_global(bool global)
    {
        if (global) {
            data = data | GLOBAL;
        } else {
            data = data & ~GLOBAL;
        }
    }

    void set_address(uintptr_t address)
    {
        assert_is_page_aligned(address);
        data = data & ADDRESS_ZERO_MASK;
        const uint64_t field = (address & ~ADDRESS_ZERO_MASK);
        data = data | field;
    }

    uint64_t get_address() const
    {
        uint64_t address = (data & ~ADDRESS_ZERO_MASK);
        return address;
    }
};

} // namespace x86_64

#endif // FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_PAGE_TABLE_ENTRY_HPP
