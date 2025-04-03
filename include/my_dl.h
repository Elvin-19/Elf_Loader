#pragma once

#include <stdio.h>

extern void *my_dlopen(const char *name);
extern int *my_dlsym(void *handle, const char *symbol);