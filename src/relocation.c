
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "phdr.h"
#include "relocation.h"

static uint32_t get_segment_protection_from_addr(uint64_t offset, elf64_phdr **phdr_tab,
                                                 int nb_load_segments) {
    for (int i = 0; i < nb_load_segments; i++) {
        elf64_phdr *current = phdr_tab[i];
        if ((offset >= current->vaddr) && (offset <= (current->vaddr + current->memsz))) {
            return current->flags;
        }
    }
    return 0;
}

void dynamic_relocation(int lib_fd, elf64_ehdr *ehdr, void *load_addr, elf64_phdr **phdr_tab,
                        int nb_load_segments) {
    elf64_phdr dyn_header;
    elf64_dyn *dyn_entrie;

    elf64_rela *rela = NULL;
    size_t rela_size;
    size_t rela_entry_size = sizeof(elf64_rela);

    // Find the dynamic header
    lseek(lib_fd, ehdr->phoff, SEEK_SET);
    for (int i = 0; i < ehdr->phnum; i++) {
        read(lib_fd, &dyn_header, sizeof(elf64_phdr));
        if (dyn_header.type == PT_DYNAMIC) {
            dyn_entrie = (elf64_dyn *) ((uint64_t) load_addr + dyn_header.offset);
            break;
        }
        lseek(lib_fd, ehdr->phentsize - sizeof(elf64_phdr), SEEK_CUR);
    }
    if (debug == true) {
        printf("[ DEBUG ] Dynamic header found at offset %lx\n", dyn_header.offset);
    }

    // Loop over all the enrties in the Dynamic section
    if (debug == true) {
        printf("[ DEBUG ] ----------- LIST OF DYNAMIC ENTRIES -----------\n");
    }
    for (elf64_dyn *current = dyn_entrie; current->d_tag != DT_NULL; current++) {
        if (debug == true) {
            printf("[ DEBUG ] Dynamic entry: 0x%lx , Value 0x%lx\n", current->d_tag,
                   current->d_un.d_val);
        }
        switch (current->d_tag) {
        case DT_RELA:
            rela = (elf64_rela *) ((uint64_t) load_addr + current->d_un.d_ptr);
            break;
        case DT_RELASZ:
            rela_size = current->d_un.d_val;
            break;
        case DT_RELAENT:
            rela_entry_size = current->d_un.d_val;
            break;
        default:
            break;
        }
    }
    if (debug == true) {
        printf("[ DEBUG ] ----------- \n");
        printf("[ DEBUG ] Address of .rela.dyn section : %p\n", rela);
        printf("[ DEBUG ] Nb of .rela.dyn entries : %ld\n", rela_size / rela_entry_size);
    }

    // Loop over rela entries
    for (int i = 0; i < (int) (rela_size / rela_entry_size); i++) {
        if ((rela[i].r_info == R_X86_64_RELATIVE) || (rela[i].r_info == R_X86_64_RELATIVE)) {
            printf("[ DEBUG ] I'm IN %d \n", i);
            printf("[ DEBUG ] r_offset : 0x%lx\n", rela[i].r_offset);
            printf("[ DEBUG ] r_info   : 0x%lx\n", rela[i].r_info);
            printf("[ DEBUG ] r_addend : 0x%lx\n", rela[i].r_addend);
            uint64_t new_addr = (uint64_t) load_addr + rela[i].r_offset;
            uint64_t new_val = (uint64_t) load_addr + rela[i].r_addend;

            uint32_t prot =
                get_segment_protection_from_addr(rela[i].r_offset, phdr_tab, nb_load_segments);
            if (prot & PF_W) {
                memcpy((void *) new_addr, &new_val, sizeof(uint64_t));
            }
            else {
                uint64_t alligned_addr = new_addr - (new_addr % sysconf(_SC_PAGESIZE));
                if (mprotect((void *) alligned_addr, sysconf(_SC_PAGESIZE), prot | PROT_WRITE) ==
                    -1) {
                    perror("Error while changing the protection");
                    exit(EXIT_ERROR);
                }
                memcpy((void *) new_addr, &new_val, sizeof(uint64_t));
                if (mprotect((void *) alligned_addr, sysconf(_SC_PAGESIZE), prot) == -1) {
                    perror("Error while changing the protection");
                    exit(EXIT_ERROR);
                }
            }
        }
    }
}