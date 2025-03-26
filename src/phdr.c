#include "phdr.h"

uint64_t phdr_parse(int lib_fd, elf64_ehdr *ehdr, elf64_phdr **phdr_tab) {
    int is_one_load = 0;
    elf64_phdr *phdr;
    lseek(lib_fd, ehdr->phoff, SEEK_SET);
    for (int i = 0; i < ehdr->phnum; i++) {

        phdr = malloc(sizeof(elf64_phdr));
        if (phdr == NULL) {
            dprintf(STDERR_FILENO, "Error while allocating memory for the program header\n");
            exit(EXIT_FAILURE);
        }
        int read_size = read(lib_fd, phdr, sizeof(elf64_phdr));
        printf("%d, %d\n", i, read_size);
        if (read_size != ehdr->phentsize) {
            dprintf(STDERR_FILENO,
                    "Error while reading the ELF program header : Incorrect size (%d, %d) \n",
                    read_size, ehdr->phentsize);
            exit(EXIT_FAILURE);
        }
        phdr_tab[i] = phdr;
        if (phdr_tab[i]->type == PT_LOAD) {
            // First program header
            if (!is_one_load) {
                // Verifies that the first PT_LOAD segment spans over all segments headers
                if (!(((phdr_tab[i]->filesz + phdr_tab[i]->offset) >
                       (ehdr->phoff + ehdr->phnum * ehdr->phentsize)) &&
                      (phdr_tab[i]->offset < ehdr->phoff))) {

                    dprintf(STDERR_FILENO, "Not a valid binary : First PT_LOAD segment doesn't "
                                           "spans over all segments headers");
                    exit(EXIT_FAILURE);
                }
                is_one_load = 1;
            }
            // We don't need to make the following checks for the first segment
            if (i == 0)
                continue;

            // Verifies that the segments are in ascending order of vaddr
            if (phdr_tab[i]->vaddr < phdr_tab[i - 1]->vaddr) {
                dprintf(STDERR_FILENO,
                        "Not a valid binary : The file contains overlapping segments.\n");
                exit(EXIT_FAILURE);
            }
            // Verifies that the segments are not overlapping in file
            if (phdr_tab[i]->offset < (phdr_tab[i - 1]->offset + phdr_tab[i - 1]->filesz)) {
                dprintf(STDERR_FILENO,
                        "Not a valid binary : The file contains overlapping segments in file.\n");
                exit(EXIT_FAILURE);
            }
            // Verifies that the segments are not overlapping in memory
            if (phdr_tab[i]->vaddr < (phdr_tab[i - 1]->vaddr + phdr_tab[i - 1]->memsz)) {
                dprintf(STDERR_FILENO,
                        "Not a valid binary : The file contains overlapping segments in memory.\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    if (!is_one_load) {
        dprintf(STDERR_FILENO,
                "Not a valid binary : The file doesn't contains any loadable segments.\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}

void phdr_print(elf64_phdr *header) {
    printf("Program Header:\n");
    printf("Type:                              %u\n", header->type);
    printf("Flags:                             %u\n", header->flags);
    printf("Offset:                            %lu\n", header->offset);
    printf("Virtual address:                   %lu\n", header->vaddr);
    printf("Physical address:                  %lu\n", header->paddr);
    printf("File size:                         %lu\n", header->filesz);
    printf("Memory size:                       %lu\n", header->memsz);
    printf("Alignment:                         %lu\n", header->align);
}