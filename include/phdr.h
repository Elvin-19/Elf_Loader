/**
 * @file phdr.h
 * @brief This file contains defitions for the programs headers
 */

#pragma once
#include <stdio.h>

#include "ehdr.h"

#define PT_LOAD    1 /* Loadable program segment */
#define PT_DYNAMIC 2 /* Dynamic linking information */

typedef struct {
    uint32_t type;
    uint32_t flags;
    uint64_t offset;
    uint64_t vaddr;
    uint64_t paddr;
    uint64_t filesz;
    uint64_t memsz;
    uint64_t align;
} elf64_phdr;

extern int phdr_count_load_segments(int lib_fd, elf64_ehdr *ehdr);
extern uint64_t phdr_parse(int lib_fd, int nb_load_seg, elf64_ehdr *ehdr, elf64_phdr **phdr_tab);
extern void phdr_print(elf64_phdr *header);