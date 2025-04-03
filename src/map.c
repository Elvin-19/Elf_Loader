#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "common.h"
#include "ehdr.h"
#include "map.h"

void load_segment(int fd_lib, elf64_phdr **p_headers, int nb_phdr) {
    // We take the size of a page
    // size_t page_size = sysconf(_SC_PAGESIZE);

    /*
    ATTENTION : ON CONSIDERE QUE LE TABLEAU NE CONTIENT QUE LES LOADS SEG
    */
    // Compute the total size of the necessary memory to load every load segemnts
    // uint64_t min_bound = p_headers[0]->vaddr;
    // uint64_t max_bound = p_headers[nb_phdr - 1]->vaddr + p_headers[nb_phdr - 1]->memsz;

    // Align the bounds to the page size
    // uint64_t min_alligned = min_bound - (min_bound % page_size);
}