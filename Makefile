CC = gcc
CFLAGS = -std=c17 -Wall -D_POSIX_C_SOURCE=200809L -I./src/include
SRCS = src/main.c src/finder.c src/gitwrap.c src/dynamic_array.c
OBJS = $(SRCS:.c=.o)
TARGET = build/gitwhen

all: $(TARGET)

$(TARGET): $(SRCS)
	mkdir -p build
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)


FORMAT_FILES = $(shell find . -name '*.c' -o -name '*.h')
format:
	@command -v clang-format >/dev/null 2>&1 || { echo >&2 "clang-format not installed."; exit 1; }
	clang-format -i $(FORMAT_FILES)

clean:
	rm -f $(TARGET) src/*.o
