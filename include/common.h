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

// Protections flags
#define PF_X 0x1
#define PF_W 0x2
#define PF_R 0x4

// Error code
#define EXIT_SUCCESS 0
#define EXIT_ERROR   1

#define ERR_ELF_FORMAT -1
#define ERR_LOADER     -2
#define ERR_FILE       -3

extern bool debug;