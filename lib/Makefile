CC = gcc
CFLAGS = -fPIC -nostdlib -fvisibility=hidden
LDFLAGS = -shared
LINKINGFLAGS = -Wl,-e,entry
TARGET = libfoobar.so
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(SRCS)
	@as encrypted_lib.S -o encrypted_lib.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LINKINGFLAGS)

clean:
	rm -f encrypted_lib.o
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean