CC = gcc
CFLAGS = -Wall -Wextra -g -O0

SRCS_COMMON = list.c tree.c exec.c 
OBJS_COMMON = $(SRCS_COMMON:.c=.o)

all: mysh tree_only

mysh: main_full.c $(OBJS_COMMON)
	$(CC) $(CFLAGS) main_full.c $(OBJS_COMMON) -o mysh 

tree_only: main_tree.c list.o only_tree.o
	$(CC) $(CFLAGS) main_tree.c list.o only_tree.o -o tree_only

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f mysh tree_only *.o
