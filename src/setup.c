#include <stdio.h>
#include <foobar.h>

#include "setup.h"

void imported(void) {
    printf("Imported function baby ! \n");
}

int main(int argc, char *argv[]) {
    printf("%s\n", foo_exported());
    printf("%s\n", bar_exported());
    foo_imported();
    bar_imported();
    return 0;
}