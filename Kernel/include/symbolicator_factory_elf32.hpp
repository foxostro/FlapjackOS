#ifndef FLAPJACKOS_COMMON_INCLUDE_SYMBOLICATOR_FACTORY_ELF32_HPP
#define FLAPJACKOS_COMMON_INCLUDE_SYMBOLICATOR_FACTORY_ELF32_HPP

#include <generic_symbolicator_factory.hpp>
#include <common/elf32.hpp>

class SymbolicatorFactoryElf32 : public GenericSymbolicatorFactory<Elf::Elf32_Shdr, Elf::Elf32_Sym> {
public:
    using ElfSymbol = Elf::Elf32_Sym;

    SymbolicatorFactoryElf32(HardwareMemoryManagementUnit& mmu,
                             multiboot_info_t* mb_info)
     : GenericSymbolicatorFactory(mmu, mb_info)
    {}

    bool is_elf_symbol_a_function(const ElfSymbol& elf_symbol) override
    {
        return ELF32_ST_TYPE(elf_symbol.st_info) == Elf::STT_FUNC;
    }
};

#endif // FLAPJACKOS_COMMON_INCLUDE_SYMBOLICATOR_FACTORY_ELF32_HPP
