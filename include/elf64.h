/**
 * @file elf64.h
 * @brief This header contains all the structures and constants needed to manipulate ELF64 files.
 */

#pragma once

#include "common.h"

/**
 * CONSTANTS
 */

/* Values of the types of the segments */
#define PT_LOAD    1
#define PT_DYNAMIC 2

/* Values for protection flags */
#define PF_X 0x1
#define PF_W 0x2
#define PF_R 0x4

/* Values for dynamic entry types */
#define DT_NULL      0
#define DT_RELA      7
#define DT_RELASZ    8
#define DT_RELAENT   9
#define DT_RELACOUNT 0x6ffffff9

/* Relœocations types */
#define R_X86_64_RELATIVE  8
#define R_AARCH64_RELATIVE 1027

/**
 * STRUCTURES
 */

/* ELF64 Executable header structure */
typedef struct {
    unsigned char ident[16];   // Magic number and other info
    uint16_t type;             // Type of file
    uint16_t machine;          // Machine architecture
    uint32_t version;          // Version of the file
    uint64_t entry;            // Entry point address
    uint64_t phoff;            // Program header offset
    uint64_t shoff;            // Section header offset
    uint32_t flags;            // Processor-specific flags
    uint16_t ehsize;           // Exec header size
    uint16_t phentsize;        // Size of program header entry
    uint16_t phnum;            // Number of program headers
    uint16_t shentsize;        // Section header size
    uint16_t shnum;            // Number of section headers
    uint16_t shstrndx;         // Section header string table index
} elf64_ehdr;

/* ELF64 Program header structure */
typedef struct {
    uint32_t type;     // Type of segment
    uint32_t flags;    // Segment attributes
    uint64_t offset;   // Offset in file
    uint64_t vaddr;    // Virtual address in memory
    uint64_t paddr;    // Physical address (not used)
    uint64_t filesz;   // Size of segment in file
    uint64_t memsz;    // Size of segment in memory
    uint64_t align;    // Alignment of segment
} elf64_phdr;

/* ELF64 Dynamic entry structure */
typedef struct {
    int64_t d_tag; /* Dynamic entry type */
    union {
        uint64_t d_ptr; /* Address value */
        uint64_t d_val; /* Integer value */
    } d_un;
} elf64_dyn;

/* ELF64 Relocation entry structure */
typedef struct {
    uint64_t r_offset; /* Address */
    uint64_t r_info;   /* Relocation type and symbol index */
    uint64_t r_addend; /* Addend */
} elf64_rela;
