/**
 * @file e_header_parser.c
 * @brief Implementation of the ELF header parser
 */

#include "ehdr.h"

elf64_ehdr ehdr_parse(int lib_fd) {
    elf64_ehdr headerLib;

    // Reading the content of the file
    if (read(lib_fd, &headerLib, sizeof(elf64_ehdr)) != sizeof(elf64_ehdr)) {
        perror("Error while reading the ELF executable header\n");
        exit(EXIT_FAILURE);
    }

    // Making sure that the lib is ELF, 64bits, and dynamic

    if (headerLib.ident[0] != 0x7f || headerLib.ident[1] != 'E' || headerLib.ident[2] != 'L' ||
        headerLib.ident[3] != 'F') {
        dprintf(STDERR_FILENO,
                "Not a valid ELF formated library : The library is not an ELF file.\n");
        exit(EXIT_FAILURE);
    }

    if (headerLib.ident[4] != 2) {
        dprintf(STDERR_FILENO,
                "Not a valid ELF formated library : The library is not a 64bits ELF file.\n");
        exit(EXIT_FAILURE);
    }
    if (headerLib.type != 3) {
        dprintf(STDERR_FILENO,
                "Not a valid ELF formated library : The library is not a dynamic ELF file.\n");
        exit(EXIT_FAILURE);
    }

    if (sizeof(elf64_ehdr) != headerLib.ehsize) {
        dprintf(STDERR_FILENO,
                "Not a valid ELF formated library : The size of the header is not correct.\n");
        exit(EXIT_FAILURE);
    }
    if (headerLib.phnum <= 0) {
        dprintf(STDERR_FILENO,
                "Not a valid ELF formated binary : The file doesn't contains any segments.\n");
        exit(EXIT_FAILURE);
    }

    return headerLib;
}

void ehdr_print(elf64_ehdr *header) {
    printf("ELF Executable Header:\n");
    printf("Magic:   ");
    for (int i = 0; i < 16; i++)
        printf("%02x ", header->ident[i]);
    printf("\n");
    printf("Type:                              %u\n", header->type);
    printf("Machine:                           %u\n", header->machine);
    printf("Version:                           %u\n", header->version);
    printf("Entry point address:               %lu\n", header->entry);
    printf("Start of program headers:          %lu\n", header->phoff);
    printf("Start of section headers:          %lu\n", header->shoff);
    printf("Flags:                             %u\n", header->flags);
    printf("Size of this header:               %u\n", header->ehsize);
    printf("Size of program headers:           %u\n", header->phentsize);
    printf("Number of program headers:         %u\n", header->phnum);
    printf("Size of section headers:           %u\n", header->shentsize);
    printf("Number of section headers:         %u\n", header->shnum);
    printf("Section header string table index: %u\n", header->shstrndx);
}