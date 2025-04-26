/**
 * @file ehdr.c
 * @brief Implementation of the ELF header parser
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ehdr.h"

elf64_ehdr ehdr_parse(int lib_fd) {
    elf64_ehdr headerLib;

    // Reading the content of the file
    if (read(lib_fd, &headerLib, sizeof(elf64_ehdr)) != sizeof(elf64_ehdr)) {
        perror("Error while reading the ELF executable header\n");
        exit(ERR_ELF_EHDR);
    }

    // Verifying that the lib is ELF, 64bits, and dynamic
    if (headerLib.ident[0] != 0x7f || headerLib.ident[1] != 'E' || headerLib.ident[2] != 'L' ||
        headerLib.ident[3] != 'F') {
        dprintf(STDERR_FILENO,
                "Not a valid ELF formated library : The library is not an ELF file.\n");
        exit(ERR_ELF_EHDR);
    }
    if (headerLib.ident[4] != 2) {
        dprintf(STDERR_FILENO,
                "Not a valid ELF formated library : The library is not a 64bits ELF file.\n");
        exit(ERR_ELF_EHDR);
    }
    if (headerLib.type != 3) {
        dprintf(STDERR_FILENO,
                "Not a valid ELF formated library : The library is not a dynamic ELF file.\n");
        exit(ERR_ELF_EHDR);
    }

    // Verifying that the executable header size is equal to the size of the structure
    if (sizeof(elf64_ehdr) != headerLib.ehsize) {
        dprintf(STDERR_FILENO,
                "Not a valid ELF formated library : The size of the header is not correct.\n");
        exit(ERR_ELF_EHDR);
    }

    // Verifying that the number of program headers is strictly positive
    if (headerLib.phnum <= 0) {
        dprintf(STDERR_FILENO,
                "Not a valid ELF formated binary : The file doesn't contains any segments.\n");
        exit(ERR_ELF_EHDR);
    }

    return headerLib;
}

void ehdr_print(elf64_ehdr *header) {
    printf("%sELF Executable Header:\n", (debug ? "[ DEBUG ] " : ""));
    printf("%s    Magic:   ", (debug ? "[ DEBUG ] " : ""));
    for (int i = 0; i < 16; i++)
        printf("%02x ", header->ident[i]);
    printf("\n");
    printf("%s    Type                       : %u\n", (debug ? "[ DEBUG ] " : ""), header->type);
    printf("%s    Machine                    : %u\n", (debug ? "[ DEBUG ] " : ""), header->machine);
    printf("%s    Version                    : %u\n", (debug ? "[ DEBUG ] " : ""), header->version);
    printf("%s    Entry point address        : %lu\n", (debug ? "[ DEBUG ] " : ""), header->entry);
    printf("%s    Start of program headers   : %lu\n", (debug ? "[ DEBUG ] " : ""), header->phoff);
    printf("%s    Start of section headers   : %lu\n", (debug ? "[ DEBUG ] " : ""), header->shoff);
    printf("%s    Flags                      : %u\n", (debug ? "[ DEBUG ] " : ""), header->flags);
    printf("%s    Size of this header        : %u\n", (debug ? "[ DEBUG ] " : ""), header->ehsize);
    printf("%s    Size of program headers    : %u\n", (debug ? "[ DEBUG ] " : ""),
           header->phentsize);
    printf("%s    Number of program headers  : %u\n", (debug ? "[ DEBUG ] " : ""), header->phnum);
    printf("%s    Size of section headers    : %u\n", (debug ? "[ DEBUG ] " : ""),
           header->shentsize);
    printf("%s    Number of section headers  : %u\n", (debug ? "[ DEBUG ] " : ""), header->shnum);
    printf("%s    Section hdr string tab idx : %u\n", (debug ? "[ DEBUG ] " : ""),
           header->shstrndx);
}