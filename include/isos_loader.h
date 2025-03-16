#pragma once 

struct arguments {
    char *lib;
    char **functions;
    int nb_functions;
    int verbose;
};

typedef char* (*foobar_t)();
