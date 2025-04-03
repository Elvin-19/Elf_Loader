#include <dlfcn.h>

#include "my_dl.h"

void *my_dlopen(const char *name) { return dlopen(name, 1); }
int *my_dlsym(void *handle, const char *symbol) { return dlsym(handle, symbol); }