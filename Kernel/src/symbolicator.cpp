#include <symbolicator.hpp>
#include <cassert>

extern "C" char* __cxa_demangle_gnu3(const char* mangled_name);

void Symbolicator::register_symbol(const Symbol& symbol)
{
    registered_symbols_.push_back(symbol);
}

const char* Symbolicator::symbolicate(const void* instruction_pointer)
{
    const uintptr_t ip = reinterpret_cast<uintptr_t>(instruction_pointer);

    assert(registered_symbols_.size() > 0);
    const Symbol* best = nullptr;
    
    for (int i = 0; i < registered_symbols_.size(); ++i) {
        const Symbol& symbol = registered_symbols_[i];
        if ((symbol.address <= ip) && (!best || (best && (symbol.address > best->address)))) {
            best = &symbol;
        }
    }
    
    if (best) {
        return demangle(best->name.get_pointer());
    } else {
        return nullptr;
    }
}

const char* Symbolicator::demangle(const char* possibly_mangled)
{
    if (possibly_mangled[0] == '_' && possibly_mangled[1] == 'Z') {
        return __cxa_demangle_gnu3(possibly_mangled);
    } else {
        return possibly_mangled;
    }
}

Symbolicator g_symbolicator;

const char* symbolicate(const void* instruction_pointer)
{
    return g_symbolicator.symbolicate(instruction_pointer);
}
