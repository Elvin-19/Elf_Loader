# 
# This Makefile is for the ISOS project and ensure compatibility with the CI.
# Make sure to include this file in your root Makefile (i.e., at the top-level of your repository).
#

# TODO
# Initialize this variable to point to the directory holding your header if any.
# Otherwise, the CI will consider the top-level directory.
INCLUDE_DIR=./include

# TODO
# Initialize this variable with a space separated list of the paths to the loader source files (not the library).
# You can use some make native function such as wildcard if you want.
SRC_FILES=./src/isos_loader.c ./src/my_dl.c ./src/e_header_parser.c

# TODO
# Uncomment this and initialize it to the correct path(s) to your source files if your project sources are not located in `src`.
#vpath %.c path/to/src

CC=gcc
CFLAGS=-I$(INCLUDE_DIR) -O2 -Warray-bounds -Wsequence-point -Walloc-zero -Wnull-dereference -Wpointer-arith -Wcast-qual -Wcast-align=strict -g


all:
	@echo "Building ISOS project..."
	@echo "Building the library..."
	@make -C ./lib all
	@echo "Building the loader..."
	@make -C ./src all
	@$(CC) $(CFLAGS) -o isos_loader $(SRC_FILES)
	@echo "ISOS project built successfully."
	
clean: 
	@echo "Cleaning ISOS project..."
	@make -C ./lib clean
	@make -C ./src clean
	@rm -f isos_loader
	@echo "ISOS project cleaned successfully."
