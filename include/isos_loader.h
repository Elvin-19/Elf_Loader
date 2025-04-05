/**
 * @file isos_loader.h
 * @brief Header file for the isos_loader program.
 */

#pragma once

#include <stdbool.h>

struct arguments {
    char *lib;
    char **functions;
    int nb_functions;
    bool debug;
};

typedef char *(*foobar_t)();
