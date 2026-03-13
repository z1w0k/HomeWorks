#include "tree.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int depth = 1;

static CommandNode* new_command(char *cmd) {
    CommandNode *c = calloc(1, sizeof(CommandNode));
    c->cmd = strdup(cmd);
    c->argc = 1;
    c->argv = calloc(2, sizeof(char*));
    c->argv[0] = strdup(cmd);
    return c;
}

static void add_arg(CommandNode *c, char *arg) {
    c->argc++;
    c->argv = realloc(c->argv, (c->argc + 1) * sizeof(char*));
    c->argv[c->argc - 1] = strdup(arg);
    c->argv[c->argc] = NULL;
}

void print_tree(CommandNode *node) {
    if (!node) return;

    printf("argc = %d\n", node->argc);
    for (int i = 0; i < node->argc; i++) {
        printf("argv[%d] = %s\n", i, node->argv[i]);
    }
    printf("input from = %s\n", node->input_file ? node->input_file : "no");
    printf("output to = %s\n", node->output_file ? node->output_file : "no");


    if (node->background) {
        printf("background mode\n");
    } else {
        printf("normal mode\n");
    }

    printf("type = %d\n", node->type);
    if (node->conv) {
        printf("conv:\n");
        printf("\tDepth = %d:\n", depth);
        depth++;
        print_tree(node->conv);
    } else {
        printf("conv = no\n");
    }

    depth = 1;

}

static void free_command(CommandNode *c) {
    if (!c) return;
    free(c->cmd);
    for (int i = 0; i < c->argc; i++) {
        free(c->argv[i]);
    }
    free(c->argv);
    free(c->input_file);
    free(c->output_file);
    free(c);
}

static CommandNode* parse_pipeline(List *lst) {
    if (!*lst) return NULL;

    CommandNode *head = NULL;
    CommandNode *tail = NULL;

    while (*lst) {
        char *tok = (*lst)->word;
        if (strcmp(tok, "|") == 0) {
            *lst = (*lst)->next;
            continue;
        }
        if (strcmp(tok, "<") == 0 || strcmp(tok, ">") == 0 || strcmp(tok, ">>") == 0) {
            if (!tail) {
                if (head) {
                    CommandNode *cur = head;
                    while (cur) {
                        CommandNode *next = cur->conv;
                        free_command(cur);
                        cur = next;
                    }
                }
                return NULL;
            }
            *lst = (*lst)->next;
            if (!*lst) {
                if (head) {
                    CommandNode *cur = head;
                    while (cur) {
                        CommandNode *next = cur->conv;
                        free_command(cur);
                        cur = next;
                    }
                }
                return NULL;
            }
            char *file = (*lst)->word;
            if (strcmp(tok, "<") == 0) {
                tail->input_file = strdup(file);
            } else if (strcmp(tok, ">") == 0) {
                tail->output_file = strdup(file);
                tail->append = false;
            } else if (strcmp(tok, ">>") == 0) {
                tail->output_file = strdup(file);
                tail->append = true;
            }
            *lst = (*lst)->next;
            continue;
        }

        if ((strcmp(tok, "&") == 0)) {
            if (!tail) {
                if (head) {
                    CommandNode *cur = head;
                    while (cur) {
                        CommandNode *next = cur->conv;
                        free_command(cur);
                        cur = next;
                    }
                }
                return NULL;
            }
            tail->background = true;
            *lst = (*lst)->next;
            continue;
        }

        CommandNode *cmd = new_command(tok);
        *lst = (*lst)->next;

        while (*lst) {
            tok = (*lst)->word;
            if (strcmp(tok, "|") == 0) {
                break;
            }
            add_arg(cmd, tok);
            *lst = (*lst)->next;
        }

        if (!head) {
            head = cmd;
        } else {
            tail->conv = cmd;
        }
        tail = cmd;
    }

    return head;
}

static CommandNode* parse_sequence(List *lst) {
    if (!*lst) return NULL;

    CommandNode *head = NULL;
    CommandNode *tail = NULL;

    while (*lst) {
        CommandNode *pipeline = parse_pipeline(lst);
        if (!pipeline) {
            if (head) {
                CommandNode *cur = head;
                while (cur) {
                    CommandNode *next = cur->next;
                    CommandNode *conv_cur = cur->conv;
                    while (conv_cur) {
                        CommandNode *conv_next = conv_cur->conv;
                        free_command(conv_cur);
                        conv_cur = conv_next;
                    }
                    free_command(cur);
                    cur = next;
                }
            }
            return NULL;
        }

        if (!head) {
            head = pipeline;
        } else {
            tail->next = pipeline;
        }

        if (*lst) {
            char *op = (*lst)->word;
            if (strcmp(op, "&&") == 0) tail->type = 1;
            else if (strcmp(op, "||") == 0) tail->type = 2;
            else if (strcmp(op, ";") == 0) tail->type = 3;
            *lst = (*lst)->next;
        }

        tail = pipeline;
        while (tail->conv) tail = tail->conv;
    }

    return head;
}

Tree* create_tree(List lst) {
    Tree *t = malloc(sizeof(Tree));
    t->root = parse_sequence(&lst);
    return t;
}

void clear_tree(Tree **tr) {
    if (!tr || !*tr) return;
    CommandNode *cur = (*tr)->root;
    while (cur) {
        CommandNode *next = cur->next;
        CommandNode *conv_cur = cur->conv;
        while (conv_cur) {
            CommandNode *conv_next = conv_cur->conv;
            free_command(conv_cur);
            conv_cur = conv_next;
        }
        free_command(cur);
        cur = next;
    }
    free(*tr);
    *tr = NULL;
}
