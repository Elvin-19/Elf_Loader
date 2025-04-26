/**
 * @file ehdr.h
 * @brief Definitions used to parse and print ELF executable headers
 */

#pragma once

#include "elf64.h"

/**
 * @fn ehdr_parse
 * @brief Parse the ELF64 executable header
 * @param lib_fd File descriptor of the ELF file
 * @return elf64_ehdr structure containing the parsed header
 */
extern elf64_ehdr ehdr_parse(int lib_fd);

/**
 * @fn ehdr_print
 * @brief Print the ELF64 executable header
 * @param header Pointer to the elf64_ehdr structure to print
 */
extern void ehdr_print(elf64_ehdr *header);