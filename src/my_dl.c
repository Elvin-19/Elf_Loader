#include <dlfcn.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ehdr.h"
#include "lib_reader.h"
#include "my_dl.h"
#include "phdr.h"
#include "relocation.h"
#include "segments_loader.h"

void *my_dlopen(char *name) {
    // Opening the libraty file
    /*
    int lib_fd = open(name, O_RDONLY);
    if (lib_fd == -1) {
        perror("Error while opening the library file\n");
        exit(ERR_LOADER);
    }
    */
    char *key = name;
    int lib_fd = decrypt_lib(key, strlen(key));
    // Parsing its executable header
    elf64_ehdr libExecHeader = ehdr_parse(lib_fd);
    if (debug == true) {
        printf("[ DEBUG ] Content of the executable header : \n");
        ehdr_print(&libExecHeader);
        printf("[ DEBUG ] -----------\n");
    }

    // Counting the number of loadable segments
    int nb_load_seg = phdr_count_load_segments(lib_fd, &libExecHeader);
    if (debug == true) {
        printf("[ DEBUG ] Number of loadable segments : %d\n", nb_load_seg);
        printf("[ DEBUG ] -----------\n");
    }

    // Parsing the program headers
    elf64_phdr **phdr_tab = malloc(nb_load_seg * sizeof(elf64_phdr *));
    uint64_t size_pt_loads = phdr_parse(lib_fd, nb_load_seg, &libExecHeader, phdr_tab);
    if (debug == true) {
        for (int i = 0; i < nb_load_seg; i++) {
            printf("[ DEBUG ] Content of program header n°%d : \n", i);
            phdr_print(phdr_tab[i]);
        }
        printf("[ DEBUG ] Size of the PT_LOAD segments : %lu\n", size_pt_loads);
        printf("[ DEBUG ] -----------\n");
    }

    // Loading the segments in the memory
    void *load_addr = load_segments(lib_fd, phdr_tab, nb_load_seg, size_pt_loads);
    printf("Segments loaded in memory at address : %p\n", load_addr);

    // Performing the dynamic relocation
    dynamic_relocation(lib_fd, &libExecHeader, load_addr, phdr_tab, nb_load_seg);

    // Get the personalised symbol table
    uint64_t *entry_ptr =
        (uint64_t *) ((uint64_t) load_addr + (libExecHeader.entry - phdr_tab[0]->vaddr));
    struct my_symtab *dynsymtab = (struct my_symtab *) *entry_ptr;

    if (debug == true) {
        printf("[ DEBUG ] Entry pointer VA (from e_entry) : 0x%lx\n", libExecHeader.entry);
        printf("[ DEBUG ] Pointer to entry symbol table : %p\n", (void *) entry_ptr);
        printf("[ DEBUG ] dynsymtab : %p\n", dynsymtab);
        printf("[ DEBUG ] foo_exported = %p\n", dynsymtab->foo_exported);
        printf("[ DEBUG ] bar_exported = %p\n", dynsymtab->bar_exported);
    }

    // Cleaning
    for (int i = 0; i < nb_load_seg; i++) {
        free(phdr_tab[i]);
    }
    free(phdr_tab);
    close(lib_fd);

    return dynsymtab;
}
void *my_dlsym(void *handle, const char *name) {
    struct my_symtab *tab = (struct my_symtab *) handle;

    if (strcmp(name, "foo_exported") == 0)
        return tab->foo_exported;
    if (strcmp(name, "bar_exported") == 0)
        return tab->bar_exported;

    return NULL;
}