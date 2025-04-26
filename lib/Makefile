CC = gcc
CFLAGS = -fPIC -nostdlib -fvisibility=hidden
LDFLAGS = -shared
LINKINGFLAGS = -Wl,-e,entry
TARGET = libfoobar.so
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LINKINGFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean