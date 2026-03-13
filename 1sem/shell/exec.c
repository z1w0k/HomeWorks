#include "exec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>

volatile int flag_exit = 0;
int stat = 1;

static bool is_internal(char *cmd) {
    return strcmp(cmd, "cd") == 0 || strcmp(cmd, "pwd") == 0 || strcmp(cmd, "exit") == 0;
}

static void run_internal(CommandNode *cmd) {
    if (strcmp(cmd->cmd, "cd") == 0) {
        if (cmd->argc < 2) {
            chdir(getenv("HOME"));
        } else {
            if (chdir(cmd->argv[1]) != 0) {
                perror("cd");
            }
        }
    } else if (strcmp(cmd->cmd, "pwd") == 0) {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd))) {
            printf("%s\n", cwd);
        } else {
            perror("pwd");
        }
    } else if (strcmp(cmd->cmd, "exit") == 0) {
        flag_exit = 1;
    }
}

static int run_pipeline(CommandNode *cmd) {
    if (!cmd) return 0;

    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdin == -1 || saved_stdout == -1) {
        perror("dup");
        if (saved_stdin != -1) close(saved_stdin);
        if (saved_stdout != -1) close(saved_stdout);
        return 1;
    }

    int status = 0;
    int pipefd[2] = {-1, -1};
    CommandNode *cur = cmd;

    while (cur) {
        if (cur->conv) {
            pipefd[0] = -1;
            pipefd[1] = -1;
            if (pipe(pipefd) == -1) {
                perror("pipe");
                goto cleanup;
            }
        }

        pid_t pid = fork();
        if (pid == 0) {
            if (cur->input_file) {
                int fd = open(cur->input_file, O_RDONLY);
                if (fd < 0) {
                    perror(cur->input_file);
                    exit(1);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            } else if (cur != cmd) {
                dup2(saved_stdin, STDIN_FILENO);
            }

            if (cur->output_file) {
                int flags = O_WRONLY | O_CREAT | (cur->append ? O_APPEND : O_TRUNC);
                int fd = open(cur->output_file, flags, 0644);
                if (fd < 0) {
                    perror(cur->output_file);
                    exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            } else if (cur->conv) {
                dup2(pipefd[1], STDOUT_FILENO);
            }

            if (saved_stdin != -1) close(saved_stdin);
            if (saved_stdout != -1) close(saved_stdout);
            if (pipefd[0] != -1) close(pipefd[0]);
            if (pipefd[1] != -1) close(pipefd[1]);

            if (is_internal(cur->cmd)) {
                run_internal(cur);
                exit(0);
            } else {
                execvp(cur->cmd, cur->argv);
                fprintf(stderr, "%s: command not found\n", cur->cmd);
                exit(127);
            }
        }

        if (cur->conv) {
            if (pipefd[1] != -1) close(pipefd[1]);
            if (saved_stdin != -1) close(saved_stdin);
            saved_stdin = pipefd[0];
        } else {
            if (saved_stdin != -1) {
                close(saved_stdin);
                saved_stdin = -1;
            }
            if (saved_stdout != -1) {
                close(saved_stdout);
                saved_stdout = -1;
            }
        }

        if (!cur->background) {
            waitpid(pid, &status, 0);
        }
        cur = cur->conv;
    }

    if (saved_stdin != -1) {
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdin);
    }
    if (saved_stdout != -1) {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
    }
    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;

cleanup:
    if (saved_stdin != -1) close(saved_stdin);
    if (saved_stdout != -1) close(saved_stdout);
    if (pipefd[0] != -1) close(pipefd[0]);
    if (pipefd[1] != -1) close(pipefd[1]);
    return 1;
}

void execute_tree(Tree *tr) {
    if (!tr || !tr->root) return;

    CommandNode *cur = tr->root;
    int last_status = 0;

    while (cur && !flag_exit) {

        if (is_internal(cur->cmd)) {
            run_internal(cur);
            last_status = flag_exit ? 0 : (errno ? 1 : 0);
        } else {
            last_status = run_pipeline(cur);
        }

        if (cur->type == 1 && last_status != 0) break; 
        if (cur->type == 2 && last_status == 0) break; 

        cur = cur->next;
    }
}
