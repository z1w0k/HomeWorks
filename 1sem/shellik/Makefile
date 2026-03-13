CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = myshell
SOURCES = main.c tokens.c parser.c variables.c
HEADERS = tokens.h parser.h variables.h
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean
