# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g -I../include

# Source files
SRCS = $(wildcard *.c)

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(OBJS)

# Rule to create object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJS)