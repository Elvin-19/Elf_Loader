/**
 * @file phdr.h
 * @brief This file contains defitions for the programs headers
 */

#pragma once

#include "elf64.h"

/**
 * @fn phdr_count_load_segments
 * @brief Count the number of loadable segments in the ELF file
 * @note This function is used to predict the number of loadedable segments and prevent the usage of
 * the realloc function in the phdr_parse function.
 * @param lib_fd File descriptor of the ELF file
 * @param ehdr Pointer to the ELF header structure
 * @return Number of loadable segments
 */
extern int phdr_count_load_segments(int lib_fd, elf64_ehdr *ehdr);

/**
 * @fn phdr_parse
 * @brief Parse the program headers of the ELF file
 * @param lib_fd File descriptor of the ELF file
 * @param nb_load_seg Number of loadable segments
 * @param ehdr Pointer to the ELF header structure
 * @param phdr_tab Pointer to an array of loadable program headers (ONLY loadable ones).
 * @return The size between the first loadable segment and the end of the last one
 */
extern uint64_t phdr_parse(int lib_fd, int nb_load_seg, elf64_ehdr *ehdr, elf64_phdr **phdr_tab);

/**
 * @fn phdr_print
 * @brief Print an ELF program header
 * @param header Pointer to the ELF program header structure to print
 */
extern void phdr_print(elf64_phdr *header);