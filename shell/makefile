CC = gcc
CFLAGS = -Wall -Wextra -g -O0
LDFLAGS =

SRCS_COMMON = list.c tree.c exec.c
OBJS_COMMON = $(SRCS_COMMON:.c=.o)

all: mysh tree_only

mysh: main_full.c $(OBJS_COMMON)
	$(CC) $(CFLAGS) main_full.c $(OBJS_COMMON) -o mysh $(LDFLAGS)

tree_only: main_tree.c list.o tree.o
	$(CC) $(CFLAGS) main_tree.c list.o tree.o -o tree_only $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f mysh tree_only *.o
