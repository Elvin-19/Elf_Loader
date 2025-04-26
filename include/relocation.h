/**
 * @file relocation.h
 * @brief Definitions of the functions used to perform the dynamic relocation
 */

#pragma once

#include "elf64.h"

/**
 * @fn dynamic_relocation
 * @brief Perform the dynamic relocation of the ELF file
 * @param lib_fd File descriptor of the ELF file
 * @param ehdr Pointer to the ELF header structure
 * @param base_addr Base address of the loaded ELF file in memory
 * @param phdr_tab Pointer to an array of loadable program headers (ONLY loadable ones).
 * @param nb_load_segments Number of loadable segments
 */
void dynamic_relocation(int lib_fd, elf64_ehdr *ehdr, void *base_addr, elf64_phdr **phdr_tab,
                        int nb_load_segments);