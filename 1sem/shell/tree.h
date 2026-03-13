#ifndef __TREE_H__
#define __TREE_H__

#include "list.h"
#include <stdbool.h>

typedef struct command_node {
    char *cmd;
    int argc;
    char **argv;
    char *input_file;
    char *output_file;
    bool append;
    bool background;
    int type; 
    struct command_node *next;
    struct command_node *conv;
} CommandNode;

typedef struct tree {
    CommandNode *root;
} Tree;

Tree* create_tree(List lst);
void clear_tree(Tree **tr);
void print_tree(CommandNode *node);

#endif
