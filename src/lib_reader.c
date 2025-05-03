/**
 * @file lib_reader.c
 * @brief This file contains the function definitions for reading the encrypted files and decrypting
 * them.
 */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <unistd.h>

#include <sys/mman.h>

#include "lib_reader.h"

int decrypt_lib(char *key, size_t key_size) {
    size_t data_size = __stop_encrypted_lib - __start_encrypted_lib;
    void *data_ptr = (void *) __start_encrypted_lib;
    int fd_out = memfd_create("decrypted_lib", 0);
    char buffer;

    if (debug == true) {
        printf("[ DEBUG ] Encrypted library data start : %p\n", __start_encrypted_lib);
        printf("[ DEBUG ] Encrypted library data end : %p\n", __stop_encrypted_lib);
    }

    for (int i = 0; i < data_size; i++) {
        buffer = ((uint8_t *) data_ptr)[i] ^ key[i % key_size];

        write(fd_out, &buffer, sizeof(uint8_t));
    }
    lseek(fd_out, 0, SEEK_SET);
    return fd_out;
}