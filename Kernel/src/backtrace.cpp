#include <backtrace.hpp>
#include <common/logger.hpp>
#include <common/elf32.hpp>
#include <cstdlib>
#include <cassert>
#include <boot.h> // for KERNEL_VIRTUAL_START_ADDR

extern "C" char *__cxa_demangle_gnu3(const char *);

// AFOX_TODO: Maybe move this symbol table stuff into a new class.
// AFOX_TODO: The translation from physical to logical address should be done with the MMU class.
extern size_t g_shdr_table_count; // in kernel.cpp
extern Elf::Elf32_Shdr* g_shdr_table;

static const Elf::Elf32_Shdr& get_shdr_of_type(unsigned type)
{
    const Elf::Elf32_Shdr* shdr = &g_shdr_table[0];
    for (size_t i = 0; i < g_shdr_table_count; ++i) {
        if (g_shdr_table[i].sh_type == type) {
            shdr = &g_shdr_table[i];
            break;
        }
    }
    return *shdr;
}

static const Elf::Elf32_Shdr& get_symbol_table_shdr()
{
    return get_shdr_of_type(Elf::SHT_SYMTAB);
}

static const Elf::Elf32_Shdr& get_string_table_shdr()
{
    return get_shdr_of_type(Elf::SHT_STRTAB);
}

static const char* get_string_table()
{
    auto& shdr = get_string_table_shdr();
    return reinterpret_cast<const char*>(KERNEL_VIRTUAL_START_ADDR + shdr.sh_addr);
}

static const char* get_string(size_t index)
{
    return get_string_table() + index;
}

#define ELF32_ST_TYPE(i) ((i)&0xf)
constexpr unsigned STT_FUNC = 2;

static const Elf::Elf32_Sym* get_symbol(uintptr_t ip)
{
    // Search for the function symbol with the greatest address not past the IP.
    // This is going to be the best match for the function the CPU is currently
    // executing within.
    // We do this linear scan every time because the symbols in the table are
    // not sorted.
    
    auto& shdr = get_symbol_table_shdr();
    const Elf::Elf32_Sym* symbols = reinterpret_cast<const Elf::Elf32_Sym*>(KERNEL_VIRTUAL_START_ADDR + shdr.sh_addr);
    size_t count = shdr.sh_size / sizeof(Elf::Elf32_Sym);
    const Elf::Elf32_Sym* best = &symbols[0];
    
    for (size_t i = 0; i < count; ++i) {
        const Elf::Elf32_Sym* symbol = &symbols[i];
        if (ELF32_ST_TYPE(symbol->st_info) == STT_FUNC &&
            symbol->st_value > best->st_value &&
            symbol->st_value <= ip) {

            best = symbol;
        }
    }

    return best;
}

static const char* symbolicate(const void* instruction_pointer)
{
    const uintptr_t ip = reinterpret_cast<uintptr_t>(instruction_pointer);
    const Elf::Elf32_Sym* symbol = get_symbol(ip);
    if (symbol) {
        const char* possibly_mangled = get_string(symbol->st_name);
        if (possibly_mangled[0] == '_' && possibly_mangled[1] == 'Z') {
            return __cxa_demangle_gnu3(possibly_mangled);
        } else {
            return possibly_mangled;
        }
    } else {
        return nullptr;
    }
}

static _Unwind_Reason_Code backtrace_trace(struct _Unwind_Context* context, void* param)
{
    const void* ip = reinterpret_cast<const void*>(_Unwind_GetIP(context));
    if (ip) {
        StackWalker::StackFrame frame;
        frame.instruction_pointer = ip;
        frame.symbol_name = symbolicate(ip);
        reinterpret_cast<StackWalker*>(param)->trace(frame);
        return _URC_NO_REASON;
    } else {
        return _URC_END_OF_STACK;
    }
}

void backtrace(StackWalker& stack_walker)
{
    _Unwind_Backtrace(backtrace_trace, reinterpret_cast<void*>(&stack_walker));
}
