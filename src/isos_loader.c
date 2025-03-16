#include <stdio.h>
#include <argp.h>
#include <stdlib.h>

#include "isos_loader.h"
#include "my_dl.h"

const char *argp_program_version = "isos_loader (MAUBERT Elvin) 1.0";

static char doc[] = "isos_loader (ISOS Project) -- re-implementation of dlopen and dlsym"
                    "\n\n"
                    "Arguments :\n\n"
                    "  LIBRARY      Path to the ELF file of the saherd library.\n"
                    "  FUNCTIONS    One or more functions to load from the library.\n"
                    "\n"
                    "Options :\n";


static char args_doc[] = "LIBRARY FUNCTION [FUNCTIONS...]";

static struct argp_option options[] = {
    { "verbose", 'v', 0, 0, "Produce verbose output", 0 },
    { 0 }
};


static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;

    switch (key) {
        case 'v':
            arguments->verbose = 1;
            break;
        case ARGP_KEY_ARG:
            if (state->arg_num == 0) {
                arguments->lib = arg;
            } else {
                arguments->functions[state->arg_num - 1] = arg;
            }
            break;
        case ARGP_KEY_END:
            if (state->arg_num < 2) {
                argp_usage(state);
            }
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

int main(int argc, char **argv) {

    /**
     * Arguments parsing
     */
    struct arguments arguments;
    arguments.verbose = 0;
    arguments.functions = malloc((argc - 1) * sizeof(char *));
    arguments.nb_functions = 0;
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    arguments.nb_functions = argc - 2 - arguments.verbose;
    if (arguments.verbose) {
        printf("Verbose mode activated\n");
        printf("Library   : %s\n", arguments.lib);
        printf("Number of functions : %d\n", arguments.nb_functions);
        printf("Functions :\n");
        for (int i = 0; i < arguments.nb_functions; i++) {
            printf("  - %s\n", arguments.functions[i]);
        }
    }
    

    /**
     * Loading the library and the functions
     */
    void *handle = my_dlopen(arguments.lib);
    if (handle == NULL) {
        fprintf(stderr, "Error while loading the library\n");
        return 1;
    }
    // Simply load the functions and call them
    // We assume that we load the foobar lib and its functions
    for (int i = 0; i < arguments.nb_functions; i++) {
        void *f = my_dlsym(handle, arguments.functions[i]);
        if (f == NULL) {
            fprintf(stderr, "Error while loading the function %s\n", arguments.functions[i]);
            return 1;
        }
        printf("Function %s loaded at %p\n", arguments.functions[i], f);
        foobar_t func = (foobar_t)f;
        char * str = func();
        printf("Function %s returned : %s\n", arguments.functions[i], str);
    }

    dlclose(handle);
    
    
    free(arguments.functions);
    return 0;    
}

