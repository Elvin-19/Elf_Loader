/**
 * @file lib_reader.h
 * @brief This header contains the function declarations for reading the encrypted
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "common.h"

int decrypt_lib(char *key, size_t key_size);
