/**
 * @file isos_loader.h
 * @brief Header file for the isos_loader program.
 */

#pragma once

#include <stdbool.h>

/**
 * @struct arguments
 * @brief Structure to hold the command line arguments.
 * @param lib Path to the shared library.
 * @param functions Array of function names to be loaded from the library.
 * @param nb_functions Number of functions to be loaded.
 * @param debug Flag to enable debug mode.
 */
struct arguments {
    char *lib;
    char **functions;
    int nb_functions;
    bool debug;
};

typedef char *(*foobar_t)();
