/**
 * @file common.h
 * @brief This file contains common definitions for the project
 */

#pragma once
#include <stdbool.h>

// Types
typedef __uint16_t uint16_t;
typedef __uint32_t uint32_t;
typedef __uint64_t uint64_t;

typedef __int64_t int64_t;

// Error code
#define EXIT_SUCCESS 0
#define EXIT_ERROR   1

#define ERR_ELF_EHDR     -1
#define ERR_ELF_PHDR     -2
#define ERR_ELF_SEG_LOAD -3
#define ERR_ELF_RELA     -4
#define ERR_LOADER       -5

extern bool debug;