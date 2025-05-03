/**
 * @file lib_reader.h
 * @brief This header contains the function declarations for reading the encrypted
 */
#pragma once

#include <stddef.h>
#include <stdint.h>

#include "common.h"

extern uint8_t __start_encrypted_lib[];
extern uint8_t __stop_encrypted_lib[];

int decrypt_lib(char *key, size_t key_size);