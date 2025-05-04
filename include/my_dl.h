/**
 * @file my_dl.h
 * @brief Header file for my implementation of a dynamic loader.
 */

#pragma once

/**
 * @struct my_symtab
 * @brief Structure to store the symbol table
 */
struct my_symtab {
    const char *(*foo_exported)();
    const char *(*bar_exported)();
};

/**
 * @fn my_dlopen
 * @brief Custom implementation of dlopen
 * @param name Path to the shared library
 * @return Handle to the loaded library (The address of the symbol table)
 */
extern void *my_dlopen(char *name);

/**
 * @fn my_dlsym
 * @brief Custom implementation of dlsym
 * @param handle Handle to the loaded library
 * @param symbol Name of the symbol to be loaded
 * @return Address of the symbol in memory
 */
extern void *my_dlsym(void *handle, const char *symbol);