/**
 * @file segments_loader.h
 * @brief Definitions of the functions used to load the segments in memory
 */
#pragma once

#include "phdr.h"

void *load_segments(int fd_lib, elf64_phdr **p_headers, int nb_phdr, uint64_t size_pt_loads);