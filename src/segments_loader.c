/**
 * @file segments_loader.c
 * @brief Implementation of the segments loader
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "common.h"
#include "ehdr.h"
#include "segments_loader.h"

static void print_segement_mapped(void *addr, uint64_t size) {
    printf("%sMapped segment :", debug ? "[ DEBUG ] " : "");
    unsigned char *ptr = (unsigned char *) addr;
    for (uint64_t i = 0; i < size; i++) {
        if (i % 16 == 0) {
            printf("\n%s", debug ? "[ DEBUG ] " : "");
        }
        printf("%02x ", *(ptr + i));
    }
    printf("\n");
}

void *load_segments(int fd_lib, elf64_phdr **p_headers, int nb_phdr, uint64_t size_pt_loads) {
    // We take the size of a page
    size_t page_size = sysconf(_SC_PAGESIZE);

    // We allign the size to the page size
    // NB: This is "useless" because mmap allocate automatically the full page
    size_t alligned_size = size_pt_loads + (page_size - (size_pt_loads % page_size));

    void *base_addr =
        mmap((void *) 0x00, alligned_size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (base_addr == MAP_FAILED) {
        perror("Error while mapping the memory");
        exit(EXIT_ERROR);
    }

    if (debug == true) {
        printf("[ DEBUG ] ----------- START OF MAPPING -----------\n");
        printf("[ DEBUG ] Page size     : %lx\n", page_size);
        printf("[ DEBUG ] size_pt_loads : %lx\n", size_pt_loads);
        printf("[ DEBUG ] Total size    : %lx\n", alligned_size);
        printf("[ DEBUG ] Base address  : 0x%p\n", base_addr);
        printf("[ DEBUG ] ----------- \n");
    }

    for (int i = 0; i < nb_phdr; i++) {
        // Take the address and sizes
        uint64_t vaddr = p_headers[i]->vaddr;
        uint64_t memsz = p_headers[i]->memsz;
        uint64_t filesz = p_headers[i]->filesz;
        uint64_t offset = p_headers[i]->offset;

        // Allign the address to the page size
        uint64_t alligned_vaddr = vaddr - (vaddr % page_size);
        uint64_t allign_offset = offset - (offset % page_size);
        uint64_t extra_offset = vaddr - alligned_vaddr;
        alligned_vaddr += (uint64_t) base_addr;

        uint64_t map_size =
            extra_offset + filesz + (page_size - (extra_offset + filesz) % page_size);

        void *addr = mmap((void *) alligned_vaddr, map_size, PROT_NONE, MAP_PRIVATE | MAP_FIXED,
                          fd_lib, allign_offset);

        if (addr == MAP_FAILED) {
            perror("Error while mapping the memory");
            printf("Ernno = %d\n", errno);
            exit(EXIT_ERROR);
        }

        // Apply the protection
        int prot = PROT_NONE;
        prot |= (p_headers[i]->flags & PF_R) ? PROT_READ : 0;
        prot |= (p_headers[i]->flags & PF_W) ? PROT_WRITE : 0;
        prot |= (p_headers[i]->flags & PF_X) ? PROT_EXEC : 0;
        if (mprotect(addr, map_size, prot) == -1) {
            perror("Error while applying the protection");
            exit(EXIT_ERROR);
        }

        // It means that this is the bss segment, we init it at 0
        if (memsz > filesz) {
            for (uint64_t i = filesz; i < memsz; i++) {
                *((char *) addr + extra_offset + i) = 0x00;
            }
        }

        // Adjust the start of the segment if it is not aligned
        if (extra_offset != 0x00) {
            // TRY PROTECT NONE
            for (uint64_t i = 0; i < extra_offset; i++) {
                *((char *) addr + i) = 0x00;
            }
        }

        if (debug == true) {
            printf("[ DEBUG ] vaddr          : 0x%lx\n", vaddr);
            printf("[ DEBUG ] memsz          : 0x%lx\n", memsz);
            printf("[ DEBUG ] offset         : 0x%lx\n", offset);
            printf("[ DEBUG ] filesz         : 0x%lx\n", filesz);
            printf("[ DEBUG ] alligned_vaddr : 0x%lx\n", alligned_vaddr);
            printf("[ DEBUG ] allign_offset  : 0x%lx\n", allign_offset);
            printf("[ DEBUG ] extra_offset   : 0x%lx\n", extra_offset);
            printf("[ DEBUG ] map_size       : 0x%lx\n", map_size);
            printf("[ DEBUG ] Final addr     : %p\n", addr);
            printf("[ DEBUG ] protections    : 0x%x\n", prot);
            print_segement_mapped((void *) ((uint64_t) addr + extra_offset), memsz);
            printf("[ DEBUG ] -----------\n");
        }
    }
    return base_addr;
}
