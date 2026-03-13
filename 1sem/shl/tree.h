#ifndef TREE_H
#define TREE_H

#include "list.h"

enum type_of_next {
    NXT, AND, OR
};

typedef struct comm_inf *tree;
typedef struct comm_inf {
    list argv;
    
    word infile;
	word outfile;
    int append;
    
    int backgrnd;
    
    tree psubcmd;
    tree pipe;
    
    enum type_of_next type;
    tree next;
} node;

tree build_tree(list);
void free_tree(tree);
void print_tree(tree, int);

#endif
