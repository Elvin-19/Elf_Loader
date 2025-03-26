/**
 * @file phdr.h
 * @brief This file contains defitions for the programs headers
 */

#pragma once
#include <fcntl.h>
#include <stdio.h>

#include "ehdr.h"

typedef __uint16_t uint16_t;
typedef __uint32_t uint32_t;
typedef __uint64_t uint64_t;

#define PT_LOAD 1 /* Loadable program segment */

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

extern uint64_t phdr_parse(int lib_fd, elf64_ehdr *ehdr, elf64_phdr **phdr_tab);
extern void phdr_print(elf64_phdr *header);