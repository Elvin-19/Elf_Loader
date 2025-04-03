/**
 * @file common.h
 * @brief This file contains common definitions for the project
 */

#pragma once

#include <stdbool.h>

// Error code
#define EXIT_SUCCESS 0
#define EXIT_ERROR   1

#define ERR_ELF_FORMAT -1
#define ERR_LOADER     -2
#define ERR_FILE       -3

extern bool debug;