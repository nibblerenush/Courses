#include <fstream>
#include <iostream>

#define ELF_NIDENT 16

// program header-ы такого типа нужно загрузить в
// память при загрузке приложения
#define PT_LOAD 1

// структура заголовка ELF файла
struct elf_hdr {
	std::uint8_t e_ident[ELF_NIDENT];
	std::uint16_t e_type;
	std::uint16_t e_machine;
	std::uint32_t e_version;
	std::uint64_t e_entry;
	std::uint64_t e_phoff;
	std::uint64_t e_shoff;
	std::uint32_t e_flags;
	std::uint16_t e_ehsize;
	std::uint16_t e_phentsize;
	std::uint16_t e_phnum;
	std::uint16_t e_shentsize;
	std::uint16_t e_shnum;
	std::uint16_t e_shstrndx;
} __attribute__((packed));

// структура записи в таблице program header-ов
struct elf_phdr {
	std::uint32_t p_type;
	std::uint32_t p_flags;
	std::uint64_t p_offset;
	std::uint64_t p_vaddr;
	std::uint64_t p_paddr;
	std::uint64_t p_filesz;
	std::uint64_t p_memsz;
	std::uint64_t p_align;
} __attribute__((packed));

std::size_t space(const char *name) {
    // Ваш код здесь, name - имя ELF файла, с которым вы работаете
    // вернуть нужно количество байт, необходимых, чтобы загрузить
    // приложение в память
    elf_hdr eh;
    std::ifstream file(name, std::ios::binary);
    file.read(reinterpret_cast<char*>(&eh), sizeof(eh));
    file.seekg(eh.e_phoff, std::ios_base::beg);

    std::size_t result = 0;
    for (std::size_t i = 0; i < eh.e_phnum; ++i) {
        elf_phdr eph;
        file.read(reinterpret_cast<char*>(&eph), sizeof(eph));
        if (eph.p_type == PT_LOAD) {
            result += eph.p_memsz;
        }
    }

    return result;
}

int main(int argc, char** argv) {
    std::cout << space(argv[0]) << std::endl;
    return 0;
}
