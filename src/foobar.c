#include "main.h"


const char* foo_exported() {
    return "foo";
}

const char* bar_exported() {
    return "bar";
}

const char* foo_imported() {
    imported();
    return "foobis";
}

const char* bar_imported() {
    imported();
    return "barbis";
}

