#pragma once

struct arguments {
    char *lib;
    char **functions;
    int nb_functions;
    int debug;
};

typedef char *(*foobar_t)();
