/**
 * @file e_header_parser.c
 * @brief Implementation of the ELF header parser
 */

#include "e_header_parser.h"

elf_e_header parser_e_header(char *lib_path) {
    int lib_fd;
    elf_e_header headerLib;
    char *buffer = NULL;

    // Opening the library file
    lib_fd = open(lib_path, O_RDONLY);
    if (lib_fd == -1) {
        perror("Error while opening the library file\n");
        exit(1);
    }

    // Allocating memory for the buffer
    buffer = malloc(sizeof(elf_e_header));
    if (buffer == NULL) {
        perror("Error while allocating memory\n");
        exit(EXIT_FAILURE);
    }

    // Reading the content of the file
    if (read(lib_fd, buffer, sizeof(elf_e_header)) <= 0) {
        perror("Error while reading the file\n");
        exit(EXIT_FAILURE);
    }

    // Copying the content of the buffer into the header
    memcpy(&headerLib, buffer, 64);

    /*
        Verifying the validity of the ELF file
    */

    // Making sure that the lib is ELF, 64bits, and dynamic

    if (headerLib.ident[0] != 0x7f || headerLib.ident[1] != 'E' || headerLib.ident[2] != 'L' ||
        headerLib.ident[3] != 'F') {
        perror("Not a valid ELF formated library : The library is not an ELF file.\n");
        exit(EXIT_FAILURE);
    }

    if (headerLib.ident[4] != 2) {
        perror("Not a valid ELF formated library : The library is not a 64bits ELF file.\n");
        exit(EXIT_FAILURE);
    }
    if (headerLib.type != 3) {
        perror("Not a valid ELF formated library : The library is not a dynamic ELF file.\n");
        exit(EXIT_FAILURE);
    }

    if (sizeof(elf_e_header) != headerLib.ehsize) {
        perror("Not a valid ELF formated library : The size of the header is not correct.\n");
        exit(EXIT_FAILURE);
    }
    if (headerLib.phnum <= 0) {
        perror("Not a valid ELF formated binary : The file doesn't contains any segments.\n");
        exit(EXIT_FAILURE);
    }

    // Freeing and closing stuff
    free(buffer);
    close(lib_fd);
    return headerLib;
}

void print_e_header(elf_e_header *header) {
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