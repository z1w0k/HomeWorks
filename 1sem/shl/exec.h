#ifndef EXEC_H
#define EXEC_H

#include "tree.h"

typedef struct backgrnd_node *b_list;
typedef struct backgrnd_node {
    pid_t pid;
    b_list next;
} b_node;

void print_backgrnd(b_list);
void clean_zombie(b_list);
int exec_comm_shell(tree);

#endif
