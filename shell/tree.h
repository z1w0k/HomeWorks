#ifndef TREE_H
#define TREE_H
#include "list.h"

typedef struct tree {
    int argc;	
    char **argv;
    char *infile;
    char *outfile;
    int append;	
    int backgrnd;
    int tnext;	
    struct tree *pipe;	
    struct tree *next;
} tree;

void error(char *);
void clear_tree(tree **);
tree *create_tree (List);
void print_tree (tree *, int);

#endif
