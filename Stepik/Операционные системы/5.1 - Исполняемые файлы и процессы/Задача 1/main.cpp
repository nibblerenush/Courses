#include <fstream>
#include <iostream>

#define ELF_NIDENT 16

// Эта структура описывает формат заголовока ELF файла
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

std::uintptr_t entry_point(const char *name) {
    elf_hdr eh;
    std::ifstream file(name, std::ios::binary);
    file.read(reinterpret_cast<char*>(&eh), sizeof(eh));
    return eh.e_entry;
}

int main(int argc, char** argv) {
    std::cout << std::hex << entry_point(argv[0]) << std::endl;
    return 0;
}
