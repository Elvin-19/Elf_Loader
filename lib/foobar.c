const char *foo_exported() { return "foo_exported"; }

const char *bar_exported() { return "bar_exported"; }

// Personallised symbol table
struct my_symtab {
    const char *(*foo_exported)();
    const char *(*bar_exported)();
};

struct my_symtab table = {
    foo_exported,
    bar_exported,
};

struct my_symtab *entry = &table;

/*
const char* foo_imported() {
    imported();
    return "foobis";
}

const char* bar_imported() {
    imported();
    return "barbis";
}

*/