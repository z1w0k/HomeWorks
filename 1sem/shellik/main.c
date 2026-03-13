#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include "tokens.h"
#include "parser.h"
#include "variables.h"

int sigint_flag = 0;
int last_exit_status = 0;

void handle_sigint(int sig)
{
    (void)sig;
    sigint_flag = 1;
    write(1, "\n", 1);
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

int builtin_cmd(struct cmd_node *cmd)
{
    if (!cmd->args[0]) return 0;
    
    if (!strcmp(cmd->args[0], "echo")) {
        int i = 1;
        int first = 1;
        while (cmd->args[i]) {
            if (!first) printf(" ");
            printf("%s", cmd->args[i]);
            first = 0;
            i++;
        }
        printf("\n");
        return 1;
    }
    
    if (!strcmp(cmd->args[0], "pwd")) {
        char dir[1024];
        if (getcwd(dir, sizeof(dir))) {
            printf("%s\n", dir);
        } else {
            perror("pwd");
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
            return 1;
        }
        return 1;
    }
    
    if (!strcmp(cmd->args[0], "exit")) {
        exit(0);
    }
    
    return 0;
}

int run_single_command(struct command *cmd)
{
    if (!cmd->pipe || !cmd->pipe->args[0]) {
        return 0;
    }
    
    expand_command_args(cmd->pipe->args);
    
    if (cmd->pipe->args[0][0] == '$') {
        fprintf(stderr, "%s: Invalid command\n", cmd->pipe->args[0]);
        return 127;
    }
    
    if (builtin_cmd(cmd->pipe)) {
        return 0;
    }
    
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    }
    
    if (pid == 0) {
        if (cmd->in_file) {
            int fd = open(cmd->in_file, O_RDONLY);
            if (fd < 0) {
                perror(cmd->in_file);
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        
        if (cmd->out_file) {
            int flags = O_WRONLY | O_CREAT;
            if (cmd->append)
                flags |= O_APPEND;
            else 
                flags |= O_TRUNC;
            int fd = open(cmd->out_file, flags, 0644);
            if (fd < 0) {
                perror(cmd->out_file);
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        
        if (cmd->bg) {
            int null_fd = open("/dev/null", O_RDONLY);
            if (null_fd >= 0) {
                dup2(null_fd, STDIN_FILENO);
                close(null_fd);
            }
        }
        
        execvp(cmd->pipe->args[0], cmd->pipe->args);
        
        fprintf(stderr, "%s: ", cmd->pipe->args[0]);
        if (errno == ENOENT) {
            fprintf(stderr, "No such file or directory\n");
        } else {
            perror("");
        }
        exit(127);
    } else {
        if (!cmd->bg) {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                return WEXITSTATUS(status);
            } else {
                return -1;
            }
        } else {
            printf("[%d]\n", pid);
            return 0;
        }
    }
}

int run_pipe(struct command *cmd)
{
    int pipe_fd[2];
    int prev_pipe = -1;
    int status = 0;
    pid_t pids[100];
    int pid_count = 0;
    struct cmd_node *node = cmd->pipe;
    int cmd_count = 0;
    
    while (node) {
        expand_command_args(node->args);
        
        if (node->args[0] && node->args[0][0] == '$') {
            fprintf(stderr, "%s: Invalid command\n", node->args[0]);
            return 127;
        }
        
        cmd_count++;
        node = node->next;
    }
    
    node = cmd->pipe;
    for (int i = 0; node; i++, node = node->next) {
        if (i < cmd_count - 1) {
            if (pipe(pipe_fd) < 0) {
                perror("pipe");
                return -1;
            }
        }
        
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return -1;
        }
        
        if (pid == 0) {
            if (!cmd->bg) {
                signal(SIGINT, SIG_DFL);
            } else {
                signal(SIGINT, SIG_IGN);
            }
            
            if (i == 0 && cmd->in_file) {
                int fd = open(cmd->in_file, O_RDONLY);
                if (fd < 0) {
                    perror(cmd->in_file);
                    exit(1);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            } else if (i > 0 && prev_pipe != -1) {
                dup2(prev_pipe, STDIN_FILENO);
                close(prev_pipe);
            }
            
            if (i == cmd_count - 1) {
                if (cmd->out_file) {
                    int flags = O_WRONLY | O_CREAT;
                    if (cmd->append)
                        flags |= O_APPEND;
                    else 
                        flags |= O_TRUNC;
                    int fd = open(cmd->out_file, flags, 0644);
                    if (fd < 0) {
                        perror(cmd->out_file);
                        exit(1);
                    }
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                } else if (prev_pipe != -1) {
                    close(pipe_fd[1]);
                }
            } else {
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }
            
            if (cmd->bg && i == 0 && !cmd->in_file) {
                int null_fd = open("/dev/null", O_RDONLY);
                if (null_fd >= 0) {
                    dup2(null_fd, STDIN_FILENO);
                    close(null_fd);
                }
            }
            
            if (prev_pipe != -1) close(prev_pipe);
            if (i < cmd_count - 1) {
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }
            
            if (!builtin_cmd(node)) {
                execvp(node->args[0], node->args);
                fprintf(stderr, "%s: ", node->args[0]);
                if (errno == ENOENT) {
                    fprintf(stderr, "No such file or directory\n");
                } else {
                    perror("");
                }
                exit(127);
            }
            exit(0);
        } else {
            pids[pid_count++] = pid;
            
            if (prev_pipe != -1) close(prev_pipe);
            if (i < cmd_count - 1) {
                close(pipe_fd[1]);
                prev_pipe = pipe_fd[0];
            }
        }
    }
    
    if (prev_pipe != -1) close(prev_pipe);
    
    if (!cmd->bg) {
        for (int i = 0; i < pid_count; i++) {
            waitpid(pids[i], &status, 0);
            if (WIFEXITED(status)) {
                last_exit_status = WEXITSTATUS(status);
            }
        }
    } else {
        printf("[%d]\n", pids[0]);
    }
    
    return last_exit_status;
}

void run_command(struct command *cmd)
{
    if (!cmd || !cmd->pipe) return;
    
    if (!cmd->pipe->next) {
        last_exit_status = run_single_command(cmd);
    } else {
        last_exit_status = run_pipe(cmd);
    }
}

void show_prompt(void)
{
    char buf[256];
    if (getcwd(buf, sizeof(buf))) {
        printf("%s$ ", buf);
    } else {
        printf("$ ");
    }
    fflush(stdout);
}

int main(void)
{
    struct tokens *t;
    struct command *cmd_list;
    struct command *cmd;
    
    signal(SIGCHLD, handle_child);
    
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    
    while (!input_finished()) {
        if (sigint_flag) {
            sigint_flag = 0;
            last_exit_status = 130;
        }
        
        show_prompt();
        t = get_tokens();
        
        if (t->count == 0) {
            free_tokens(t);
            continue;
        }
        
        if (t->count > 0 && !strcmp(t->list[0], "exit")) {
            free_tokens(t);
            break;
        }
        
        cmd_list = parse_tokens(t);
        
        cmd = cmd_list;
        while (cmd) {
            if (cmd->op == 2 && last_exit_status != 0) {
                cmd = cmd->next;
                continue;
            } else if (cmd->op == 3 && last_exit_status == 0) {
                cmd = cmd->next;
                continue;
            }
            
            run_command(cmd);
            cmd = cmd->next;
        }
        
        free_cmd(cmd_list);
        free_tokens(t);
    }
    
    return 0;
}
