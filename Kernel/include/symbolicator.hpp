#ifndef FLAPJACKOS_COMMON_INCLUDE_SYMBOLICATOR_HPP
#define FLAPJACKOS_COMMON_INCLUDE_SYMBOLICATOR_HPP

#include <common/vector.hpp>
#include <common/shared_pointer.hpp>

// Look up addresses in the kernel's symbol table.
class Symbolicator {
public:
    struct Symbol {
        uintptr_t address;
        SharedPointer<char> name;
    };

    void register_symbol(const Symbol& symbol);
    const char* symbolicate(const void* instruction_pointer);
    const char* demangle(const char* possibly_mangled);

private:
    Vector<Symbol> registered_symbols_;
};

extern Symbolicator g_symbolicator;

const char* symbolicate(const void* instruction_pointer);

#endif // FLAPJACKOS_COMMON_INCLUDE_SYMBOLICATOR_HPP
