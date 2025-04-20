#pragma once

#include <stdio.h>

struct my_symtab {
    const char *(*foo_exported)();
    const char *(*bar_exported)();
};

extern void *my_dlopen(const char *name);
extern void *my_dlsym(void *handle, const char *symbol);