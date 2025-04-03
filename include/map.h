#pragma once

#include "phdr.h"

void load_segment(int fd_lib, elf64_phdr **p_headers, int nb_phdr);