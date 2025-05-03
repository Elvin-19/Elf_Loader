/**
 * @file phdr.c
 * @brief Implementation of the ELF program header parser
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "phdr.h"

int phdr_count_load_segments(int lib_fd, elf64_ehdr *ehdr) {
    int nb_load_seg = 0;
    lseek(lib_fd, ehdr->phoff, SEEK_SET);
    uint32_t phdr_type;
    for (int i = 0; i < ehdr->phnum; i++) {
        read(lib_fd, &phdr_type, sizeof(uint32_t));
        if (phdr_type == PT_LOAD)
            nb_load_seg++;
        lseek(lib_fd, ehdr->phentsize - sizeof(uint32_t), SEEK_CUR);
    }
    return nb_load_seg;
}

uint64_t phdr_parse(int lib_fd, int nb_load_seg, elf64_ehdr *ehdr, elf64_phdr **phdr_tab) {
    elf64_phdr *phdr = NULL;

    // We need at least one loadable segment
    if (nb_load_seg == 0) {
        dprintf(STDERR_FILENO,
                "Not a valid binary : The file doesn't contains any loadable segments.\n");
        exit(ERR_ELF_PHDR);
    }

    lseek(lib_fd, ehdr->phoff, SEEK_SET);
    // Loop over all the program headers
    for (int i = 0; i < ehdr->phnum; i++) {
        // Allocating memory for the current program header
        phdr = malloc(sizeof(elf64_phdr));
        if (phdr == NULL) {
            dprintf(STDERR_FILENO, "Error while allocating memory for the program header\n");
            exit(ERR_ELF_PHDR);
        }
        // Reading the content of the file into structure
        if (read(lib_fd, phdr, sizeof(elf64_phdr)) != ehdr->phentsize) {
            dprintf(STDERR_FILENO,
                    "Error while reading the ELF program header : Incorrect size \n");
            exit(ERR_ELF_PHDR);
        }
        if (phdr->type == PT_LOAD) {
            phdr_tab[i] = phdr;

            // First program header
            if (i == 0) {
                // Verifies that the first PT_LOAD segment spans over all segments headers
                if (!(((phdr_tab[i]->filesz + phdr_tab[i]->offset) >
                       (ehdr->phoff + ehdr->phnum * ehdr->phentsize)) &&
                      (phdr_tab[i]->offset < ehdr->phoff))) {

                    dprintf(STDERR_FILENO, "Not a valid binary : First PT_LOAD segment doesn't "
                                           "spans over all segments headers");
                    exit(ERR_ELF_PHDR);
                }
                // We don't need to make the other checks for the first segment
                continue;
            }

            // Verifies that the LOADS segments are in ascending order of vaddr
            if (phdr_tab[i]->vaddr < phdr_tab[i - 1]->vaddr) {
                dprintf(STDERR_FILENO,
                        "Not a valid binary : The file contains overlapping segments.\n");
                exit(ERR_ELF_PHDR);
            }
            // Verifies that the LOADS segments are not overlapping in file
            if (phdr_tab[i]->offset < (phdr_tab[i - 1]->offset + phdr_tab[i - 1]->filesz)) {
                dprintf(STDERR_FILENO,
                        "Not a valid binary : The file contains overlapping segments in file.\n");
                exit(ERR_ELF_PHDR);
            }
            // Verifies that the LOADS segments are not overlapping in memory
            if (phdr_tab[i]->vaddr < (phdr_tab[i - 1]->vaddr + phdr_tab[i - 1]->memsz)) {
                dprintf(STDERR_FILENO,
                        "Not a valid binary : The file contains overlapping segments in memory.\n");
                exit(ERR_ELF_PHDR);
            }
        }
        else {
            // If the segment is not a PT_LOAD segment, we don't need to store it
            free(phdr);
        }
    }
    // Return the size between the first loadable segment and the end of the last one
    return (phdr_tab[nb_load_seg - 1]->vaddr + phdr_tab[nb_load_seg - 1]->memsz) -
           phdr_tab[0]->vaddr;
}

void phdr_print(elf64_phdr *header) {
    printf("%s    Type             : 0x%x\n", (debug ? "[ DEBUG ] " : ""), header->type);
    printf("%s    Flags            : 0x%x\n", (debug ? "[ DEBUG ] " : ""), header->flags);
    printf("%s    Offset           : 0x%lx\n", (debug ? "[ DEBUG ] " : ""), header->offset);
    printf("%s    Virtual address  : 0x%lx\n", (debug ? "[ DEBUG ] " : ""), header->vaddr);
    printf("%s    Physical address : 0x%lx\n", (debug ? "[ DEBUG ] " : ""), header->paddr);
    printf("%s    File size        : 0x%lx\n", (debug ? "[ DEBUG ] " : ""), header->filesz);
    printf("%s    Memory size      : 0x%lx\n", (debug ? "[ DEBUG ] " : ""), header->memsz);
    printf("%s    Alignment        : 0x%lx\n", (debug ? "[ DEBUG ] " : ""), header->align);
}