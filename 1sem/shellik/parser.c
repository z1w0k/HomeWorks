#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

struct cmd_node *add_node(struct cmd_node *first, int argc, char **argv)
{
    struct cmd_node *tmp;
    if (argc <= 0)
        return first;
        
    if (!first){
        first = malloc(sizeof(struct cmd_node));
        tmp = first;
    }
    else {
        tmp = first;
        while (tmp->next){
            tmp = tmp->next;
        }
        tmp->next = malloc(sizeof(struct cmd_node));
        tmp = tmp->next;
    }
    
    tmp->args = malloc((argc + 1) * sizeof(char *));
    int i;
    for (i = 0; i < argc; i++)
        tmp->args[i] = argv[i];
    tmp->args[argc] = NULL;
    tmp->next = NULL;
    return first;
}

struct command *parse_tokens(struct tokens *t)
{
    if (!t || t->count == 0)
        return NULL;

    struct command *cmd_list = NULL;
    struct command *current_cmd = NULL;
    struct cmd_node *current_pipe = NULL;
    
    int i = 0;
    
    while (i < t->count) {
        if (!current_cmd) {
            current_cmd = malloc(sizeof(struct command));
            memset(current_cmd, 0, sizeof(struct command));
            current_cmd->op = 0;
            
            if (!cmd_list) {
                cmd_list = current_cmd;
            } else {
                struct command *tmp = cmd_list;
                while (tmp->next) tmp = tmp->next;
                tmp->next = current_cmd;
            }
        }
        
        if (strcmp(t->list[i], ";") == 0) {
            current_cmd->op = 1;
            i++;
            current_cmd = NULL;
            continue;
        } else if (strcmp(t->list[i], "&&") == 0) {
            current_cmd->op = 2;
            i++;
            current_cmd = NULL;
            continue;
        } else if (strcmp(t->list[i], "||") == 0) {
            current_cmd->op = 3;
            i++;
            current_cmd = NULL;
            continue;
        }
        
        if (strcmp(t->list[i], "&") == 0) {
            current_cmd->bg = 1;
            i++;
            if (i >= t->count) break;
            continue;
        }
        
        if (strcmp(t->list[i], "<") == 0) {
            if (i + 1 < t->count) {
                current_cmd->in_file = strdup(t->list[i + 1]);
                i += 2;
                continue;
            }
        } else if (strcmp(t->list[i], ">") == 0) {
            if (i + 1 < t->count) {
                current_cmd->out_file = strdup(t->list[i + 1]);
                current_cmd->append = 0;
                i += 2;
                continue;
            }
        } else if (strcmp(t->list[i], ">>") == 0) {
            if (i + 1 < t->count) {
                current_cmd->out_file = strdup(t->list[i + 1]);
                current_cmd->append = 1;
                i += 2;
                continue;
            }
        }
        
        int pipe_start = i;
        while (i < t->count) {
            if (strcmp(t->list[i], "|") == 0) {
                if (pipe_start < i) {
                    current_pipe = add_node(current_pipe, i - pipe_start, t->list + pipe_start);
                }
                current_cmd->pipe = current_pipe;
                current_pipe = NULL;
                pipe_start = i + 1;
            } else if (strcmp(t->list[i], ";") == 0 || 
                      strcmp(t->list[i], "&&") == 0 || 
                      strcmp(t->list[i], "||") == 0 ||
                      strcmp(t->list[i], "&") == 0 ||
                      strcmp(t->list[i], "<") == 0 ||
                      strcmp(t->list[i], ">") == 0 ||
                      strcmp(t->list[i], ">>") == 0) {
                break;
            }
            i++;
        }
        
        if (pipe_start < i) {
            current_pipe = add_node(current_pipe, i - pipe_start, t->list + pipe_start);
        }
        
        if (current_pipe) {
            current_cmd->pipe = current_pipe;
            current_pipe = NULL;
        }
    }
    
    return cmd_list;
}

void free_cmd(struct command *cmd)
{
    struct command *next_cmd;
    while (cmd) {
        next_cmd = cmd->next;
        
        struct cmd_node *node = cmd->pipe;
        while (node) {
            struct cmd_node *next_node = node->next;
            if (node->args) {
                free(node->args);
            }
            free(node);
            node = next_node;
        }
        
        if (cmd->in_file) free(cmd->in_file);
        if (cmd->out_file) free(cmd->out_file);
        
        free(cmd);
        cmd = next_cmd;
    }
}
