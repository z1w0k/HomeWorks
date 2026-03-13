#ifndef PARSER_H
#define PARSER_H

#include "tokens.h"

struct cmd_node {
    char **args;
    struct cmd_node *next;
};

struct command {
    struct cmd_node *pipe;
    char *in_file;
    char *out_file;
    int append;
    int bg;
    struct command *next;
    int op;
};

struct command *parse_tokens(struct tokens *t);
struct cmd_node *add_node(struct cmd_node *first, int argc, char **argv);
void free_cmd(struct command *cmd);

#endif
