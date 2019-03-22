#ifndef FLAPJACKOS_COMMON_INCLUDE_GENERIC_SYMBOLICATOR_FACTORY_HPP
#define FLAPJACKOS_COMMON_INCLUDE_GENERIC_SYMBOLICATOR_FACTORY_HPP

#include <symbolicator.hpp>
#include <hardware_memory_management_unit.hpp>
#include <multiboot.h>
#include <common/elf.hpp>
#include <common/vector.hpp>
#include <common/logger.hpp>

template<typename ElfSectionHeader, typename ElfSymbol>
class GenericSymbolicatorFactory {
public:
    GenericSymbolicatorFactory(HardwareMemoryManagementUnit& mmu,
                               multiboot_info_t* mb_info)
    : mmu_(mmu), mb_info_(mb_info), shdr_table_count_(0), shdr_table_(nullptr)
    {
        assert(mb_info);
    }

    Symbolicator make_symbolicator()
    {
        multiboot_elf_section_header_table_t& elf_sec = mb_info_->u.elf_sec;
        assert(sizeof(ElfSectionHeader) == elf_sec.size);
        shdr_table_count_ = elf_sec.num;
        shdr_table_ = new ElfSectionHeader[shdr_table_count_];
        ElfSectionHeader* shdr_table = get_shdr_pointer(elf_sec.addr);
        memmove(shdr_table_, shdr_table, elf_sec.num * elf_sec.size);
        register_all_symbols();
        return std::move(symbolicator_);
    }

protected:
    virtual bool is_elf_symbol_a_function(const ElfSymbol& elf_symbol) = 0;

private:
    HardwareMemoryManagementUnit& mmu_;
    multiboot_info_t* mb_info_;
    size_t shdr_table_count_;
    ElfSectionHeader* shdr_table_;
    Symbolicator symbolicator_;

    ElfSectionHeader* get_shdr_pointer(uintptr_t physical_address)
    {
        return reinterpret_cast<ElfSectionHeader*>(mmu_.convert_physical_to_logical_address(physical_address));
    }

    void register_all_symbols()
    {
        auto& shdr = get_symbol_table_shdr();
        const ElfSymbol* elf_symbols = reinterpret_cast<const ElfSymbol*>(mmu_.convert_physical_to_logical_address(shdr.sh_addr));
        
        for (size_t i = 0, n = (shdr.sh_size / sizeof(ElfSymbol)); i < n; ++i){
            const ElfSymbol& elf_symbol = elf_symbols[i];
            if (is_elf_symbol_a_function(elf_symbol)) {
                register_elf_symbol(elf_symbol);
            }
        }
    }

    const ElfSectionHeader& get_symbol_table_shdr()
    {
        return get_shdr_of_type(Elf::SHT_SYMTAB);
    }

    const ElfSectionHeader& get_shdr_of_type(unsigned type)
    {
        const ElfSectionHeader* shdr = &shdr_table_[0];
        for (size_t i = 0; i < shdr_table_count_; ++i) {
            if (shdr_table_[i].sh_type == type) {
                shdr = &shdr_table_[i];
                break;
            }
        }
        return *shdr;
    }

    void register_elf_symbol(const ElfSymbol& elf_symbol)
    {
        assert(is_elf_symbol_a_function(elf_symbol));
        Symbolicator::Symbol symbol_to_register;
        symbol_to_register.address = elf_symbol.st_value;
        symbol_to_register.name = SharedPointer(strdup(get_string(elf_symbol.st_name)));
        symbolicator_.register_symbol(symbol_to_register);
    }

    const char* get_string(size_t index)
    {
        return get_string_table() + index;
    }

    const char* get_string_table()
    {
        auto& shdr = get_string_table_shdr();
        return reinterpret_cast<const char*>(mmu_.convert_physical_to_logical_address(shdr.sh_addr));
    }

    const ElfSectionHeader& get_string_table_shdr()
    {
        return get_shdr_of_type(Elf::SHT_STRTAB);
    }
};

#endif // FLAPJACKOS_COMMON_INCLUDE_GENERIC_SYMBOLICATOR_FACTORY_HPP
