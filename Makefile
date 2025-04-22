CC = gcc
CFLAGS = -std=c17 -Wall -I./src/include
SRCS = src/main.c src/finder.c src/gitwrap.c
OBJS = $(SRCS:.c=.o)
TARGET = build/gitwhen

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET) src/*.o
