#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "tokens.h"
#include <signal.h>

struct cmd_node{
    char **args;
    struct cmd_node *next;
};

struct command{
    struct cmd_node *pipe;
    char *in_file;
    char *out_file;
    int append;
    int bg;
};

int sigint_flag = 0;

void handle_sigint(int sig)
{
    (void)sig;
    sigint_flag = 1;
    write(1, "\n", 1);
}

int builtin_cmd(struct cmd_node *cmd)
{
    if (!cmd->args[0]) return 0;
    
    if (!strcmp(cmd->args[0], "echo")) {
        int i = 1;
        while (cmd->args[i]) {
            printf("%s", cmd->args[i]);
            if (cmd->args[i + 1]) {
                printf(" ");
            }
            i++;
        }
        printf("\n");
        return 1;
    }
    
    if (!strcmp(cmd->args[0], "pwd")) {
        char dir[1024];
        if (getcwd(dir, sizeof(dir))) {
            printf("%s\n", dir);
        }
        return 1;
    }
    
    if (!strcmp(cmd->args[0], "cd")) {
        const char *path = cmd->args[1];
        if (!path) {
            path = getenv("HOME");
        }
        if (!path) {
            fprintf(stderr, "cd: HOME not set\n");
            return 1;
        }
        if (chdir(path) < 0) {
            perror("cd");
        }
        return 1;
    }
    
    return 0;
}

void run_pipe(struct command *cmd)
{
    int fd[2], res, i, in = -1, out;
    int child_count = 0;
    pid_t pids[100];
    struct cmd_node *node;
    
    for(node = cmd->pipe; node; node = node->next) {
        
        if (!node->args[0]) continue;
        
        if (node == cmd->pipe && 
            !node->next &&
            builtin_cmd(node)) {
            return;
        }
        
        res = pipe(fd);
        if (res < 0) {
            perror("pipe");
            exit(1);
        }
        
        res = fork();
        if (res < 0) {
            perror("fork");
            exit(1);
        }
        
        if (res == 0) {
            if (!cmd->bg) {
                signal(SIGINT, SIG_DFL);
            } else {
                signal(SIGINT, SIG_IGN);
            }
            
            if (in != -1) {
                dup2(in, 0);
                close(in);
            } else if (cmd->in_file) {
                in = open(cmd->in_file, O_RDONLY);
                if (in < 0) {
                    perror(cmd->in_file);
                    exit(1);
                }
                dup2(in, 0);
                close(in);
            }
            
            if (node->next) {
                dup2(fd[1], 1);
            } else if (cmd->out_file) {
                int flags = O_WRONLY | O_CREAT;
                if (cmd->append)
                    flags |= O_APPEND;
                else 
                    flags |= O_TRUNC;
                out = open(cmd->out_file, flags, 0644);
                if (out < 0) {
                    perror(cmd->out_file);
                    exit(1);
                }
                dup2(out, 1);
                close(out);
            }
            
            close(fd[1]);
            close(fd[0]);
            
            if (!builtin_cmd(node)) {
                execvp(node->args[0], node->args);
                perror(node->args[0]);
                exit(1);
            }
            exit(0);
        }
        
        pids[child_count] = res;
        child_count++;
        
        if (in != -1)
            close(in);
        in = fd[0];
        close(fd[1]);
    }
    
    close(in);
    
    if (!cmd->bg) {
        for (i = 0; i < child_count; i++) {
            waitpid(pids[i], NULL, 0);
        }
    } else {
        printf("[%d]\n", pids[0]);
    }
}

void run_command(struct command *cmd)
{
    if (!cmd->pipe) return;
    
    if (cmd->pipe && !cmd->pipe->next) {
        if (cmd->pipe->args[0] && 
            !strcmp(cmd->pipe->args[0], "cd")) {
            
            const char *dir = cmd->pipe->args[1];
            if (!dir)
                dir = getenv("HOME");
            if (!dir) {
                fprintf(stderr, "cd: HOME not set\n");
                return;
            }
            if (chdir(dir) < 0) {
                perror("cd");
            }
            return;
        }
    }
    
    run_pipe(cmd);
}

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
    int i, start = 0, pos = 0, skip = 0;
    struct command *cmd = NULL;
    struct cmd_node *pipe = NULL;
    
    if (!t->count)
        return NULL;

    cmd = malloc(sizeof(struct command));
    memset(cmd, 0, sizeof(struct command));

    if (!strcmp(t->list[t->count - 1], "&"))
        cmd->bg = 1;

    for (i = 0; i < t->count - cmd->bg; i++){
        if (!strcmp(t->list[i], "|")){
            pipe = add_node(pipe, pos - start, t->list + start);
            start = i + 1;
            pos = i;
            skip = 0;
        }
        else if (!strcmp(t->list[i], "<") && (i != t->count - cmd->bg - 1)){
            cmd->in_file = t->list[i + 1];
            skip = 1;
        }
        else if (!strcmp(t->list[i], ">") && (i != t->count - cmd->bg - 1)){
            cmd->out_file = t->list[i + 1];
            skip = 1;
        }
        else if (!strcmp(t->list[i], ">>") && (i != t->count - cmd->bg - 1)){
            cmd->out_file = t->list[i + 1];
            cmd->append = 1;
            skip = 1;
        }
        
        if (!skip)
            pos++;
        else
            skip = 0;
    }
    
    cmd->pipe = add_node(pipe, pos - start, t->list + start);
    return cmd;
}


void free_cmd(struct command *cmd)
{
    struct cmd_node *node, *next;
    if (!cmd)
        return;
        
    node = cmd->pipe;
    while (node){
        next = node->next;
        free(node->args);
        free(node);
        node = next;
    }
    free(cmd);
}

void show_prompt(void)
{
    char buf[256];
    if (getcwd(buf, sizeof(buf)))
        printf("%s$ ", buf);
    else
        printf("$ ");
    fflush(stdout);
}

void handle_child(int sig)
{
    int status;
    pid_t pid;
    
    (void)sig;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status)) {
            printf("[%d] exited %d\n", pid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("[%d] killed by %d\n", pid, WTERMSIG(status));
        }
    }
}

int main(void)
{
    struct tokens *t;
    struct command *cmd;
    
    signal(SIGCHLD, handle_child);
    
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGINT, SIG_IGN);
    
    while (!input_finished()) {
        if (sigint_flag) {
            sigint_flag = 0;
        }
        
        show_prompt();
        t = get_tokens();
        
        if (t->count > 0 &&
            !strcmp(t->list[0], "exit")) {
            free_tokens(t);
            break;
        }
        
        cmd = parse_tokens(t);
        if (cmd) {
            run_command(cmd);
        }
        free_cmd(cmd);
        free_tokens(t);
    }
    return 0;
}
