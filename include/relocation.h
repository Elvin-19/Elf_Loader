/**
 * @file relocation.h
 * @brief Definitions of the functions used to perform the dynamic relocation
 */

#pragma once

#include "ehdr.h"

#define DT_NULL    0
#define DT_RELA    7
#define DT_RELASZ  8
#define DT_RELAENT 9

#define R_X86_64_RELATIVE  8
#define R_AARCH64_RELATIVE 1027

typedef struct {
    int64_t d_tag; /* Dynamic entry type */
    union {
        uint64_t d_ptr; /* Address value */
        uint64_t d_val; /* Integer value */
    } d_un;
} elf64_dyn;

typedef struct {
    uint64_t r_offset; /* Address */
    uint64_t r_info;   /* Relocation type and symbol index */
    uint64_t r_addend; /* Addend */
} elf64_rela;

void dynamic_relocation(int lib_fd, elf64_ehdr *ehdr, void *base_addr, elf64_phdr **phdr_tab,
                        int nb_load_segments);