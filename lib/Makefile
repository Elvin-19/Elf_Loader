CC = gcc
CFLAGS = -fPIC -Wall -Wextra -nostdlib -fvisibility=hidden -I../include
LDFLAGS = -shared
TARGET = libfoobar.so
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -Wl,-e,entry


clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean