#ifndef FLAPJACKOS_COMMON_INCLUDE_SYMBOLICATOR_FACTORY_ELF64_HPP
#define FLAPJACKOS_COMMON_INCLUDE_SYMBOLICATOR_FACTORY_ELF64_HPP

#include <generic_symbolicator_factory.hpp>
#include <common/elf64.hpp>

class SymbolicatorFactoryElf64 : public GenericSymbolicatorFactory<Elf::Elf64_Shdr, Elf::Elf64_Sym> {
public:
    using ElfSymbol = Elf::Elf64_Sym;

    SymbolicatorFactoryElf64(HardwareMemoryManagementUnit& mmu,
                             multiboot_info_t* mb_info)
     : GenericSymbolicatorFactory(mmu, mb_info)
    {}

    bool is_elf_symbol_a_function(const ElfSymbol& elf_symbol) override
    {
        return ELF64_ST_TYPE(elf_symbol.st_info) == Elf::STT_FUNC;
    }
};

#endif // FLAPJACKOS_COMMON_INCLUDE_SYMBOLICATOR_FACTORY_ELF32_HPP
