#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include "tree.h"
#include "exec.h"

#define SUCC 0
#define ERR 1

extern b_list backgrnd;

static int pid_last;

void add_backgrnd(pid_t pid)
{
    b_list tmp = (b_list)calloc(1, sizeof(b_node));
    if (tmp == NULL)
        /* MEMORY ERROR */;
    tmp->pid = pid;
    tmp->next = backgrnd;
    backgrnd = tmp;
}

void remove_backgrnd(b_list prev, b_list curr)
{
    if (prev == NULL) {
        backgrnd = curr->next;
        free(curr);
    } else {
        prev->next = curr->next;
        free(curr);
    }
}

void print_in_backgrnd(void)
{
    static unsigned i = 0;
    if (backgrnd != NULL) {
        printf("[%u] %d\n", ++i, backgrnd->pid);
    }
}

void print_backgrnd(b_list b)
{
    if (b != NULL) {
        print_backgrnd(b->next);
        printf("%d\n", b->pid);
    }
}

void free_backgrnd(b_list b)
{
    b_list tmp;
    while (b != NULL) {
        tmp = b;
        b = b->next;
        free(tmp);
    }
}

void clean_zombie(b_list b)
{
    int status, ret;
    b_list curr = b, prev = NULL, tmp;
    while (curr != NULL) {
        if ((ret = waitpid(curr->pid, &status, WNOHANG)) > 0) {
            tmp = curr->next;
            remove_backgrnd(prev, curr);
            curr = tmp;
        } else if (ret == -1) {
            printf("ERROR: [%d] can't be cleaned\n", curr->pid);
            tmp = curr->next;
            remove_backgrnd(prev, curr);
            curr = tmp;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
    
    #if 0
    while (b != NULL) {
        if (kill(b->pid, SIGTERM) != 0) {
            /* KILL */;
            kill(b->pid, SIGKILL);
        }
        b = b->next;
    }
    #endif
}

void chng_io(tree t)
{
    int in_d, out_d;
    char *in = t->infile;
    char *out = t->outfile;
    
    if (in != NULL) {
        in_d = open(in, O_RDONLY);
        if (in_d < 0) {
            printf("shell: %s: No such file or directory\n", in);
            exit(1);
        }
        dup2(in_d, 0);
        close(in_d);
    }
    
    if (out != NULL) {
        if (t->append) {
            out_d = open(out, O_WRONLY | O_CREAT | O_APPEND, 0666);
            if (out_d < 0) {
                /* ERROR */;
                exit(1);
            }
        } else {
            out_d = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (out_d < 0) {
                /* ERROR */;
                exit(1);
            }
        }
        dup2(out_d, 1);
        close(out_d);
    }
}

int exec_cd(tree t, int in_backgrnd)
{
    if (in_backgrnd)
        return ERR;
    
    char *dir_path;

    if (t->argv[1] != NULL)
        dir_path = t->argv[1];
    else {
        dir_path = getenv("HOME");
        if (dir_path == NULL)
            return ERR;
    }

    if (chdir(dir_path) == 0) {
        setenv("PWD", dir_path, 1); 
        return SUCC;
    } else {
        printf("ERROR: cd: Can't execute\n");
        return ERR;
    }
}

int exec_pwd(tree t, int in_backgrnd)
{
    if (in_backgrnd)
        return ERR;
    
    char cwd[1024];
    int out_d = 1, opened = 0;
    char *out = t->outfile;
    
    if (out != NULL) {
        if (t->append) {
            out_d = open(out, O_WRONLY | O_CREAT | O_APPEND, 0666);
            if (out_d < 0) {
                /* ERROR */;
                return ERR;
            }
            opened = 1;
        } else {
            out_d = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (out_d < 0) {
                /* ERROR */;
                return ERR;
            }
            opened = 1;
        }
    }
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        write(out_d, cwd, strlen(cwd));
        write(out_d, "\n", 1);
        if (opened)
            close(out_d);
        return SUCC;
    } else {
        printf("ERROR: pwd: Can't execute\n");
        if (opened)
            close(out_d);
        return ERR;
    }
}

int exec_clear(tree t, int in_backgrnd)
{
    if (in_backgrnd)
        return ERR;

    printf("\033[H\033[J");
    fflush(stdout); 
    return SUCC;
}

int exec_comm_shell(tree);
int exec_comm_list(tree, int, int, int, int);
int exec_conv(tree, int, int, int, int);
int exec_comm(tree, int, int, int, int);
int exec_smpl_comm(tree, int, int, int, int);

int exec_comm_shell(tree t)
{
    return exec_comm_list(t, 0, 0, 0, 1);
}

/* traverse the tree by next */
int exec_comm_list(tree t, int in, int in_pipe, int in_fd, int out_fd)
{
    int res = SUCC, in_backgrnd;
    while (t != NULL) {
        in_backgrnd = t->backgrnd;
        if (!in_backgrnd) {
            res = exec_conv(t, 0, in_pipe, in_fd, out_fd);
            switch (t->type) {
            case NXT:
                if (res == ERR)
                    printf("ERROR EXEC CONV IN LIST: start: %s\n", t->argv[0]);
                break;
            case AND:
                if (res == ERR)
                    return ERR;
                break;
            case OR:
                if (res == SUCC)
                    return SUCC;
                break;
            }
        } else
            exec_conv(t, 1, in_pipe, in_fd, out_fd); /* background */
        t = t->next;
    }
    return res; /* last res */
}

int exec_conv(tree t, int in_backgrnd, int in_pipe, int in_fd, int out_fd)
{
    if (t->pipe == NULL)
        return exec_comm(t, in_backgrnd, 0, in_fd, out_fd);
    int fd[2], status, fd_in = in_fd;
    /* in ~ fd[0], out ~ fd[1] */
    tree curr = t;
    
    while (curr != NULL) {
        if (curr->pipe != NULL) {
            if (pipe(fd) == -1) {
                printf("ERROR: pipe\n");
                return ERR;
            }
        }
        if (exec_comm(curr, in_backgrnd, 1, fd_in, (curr->pipe != NULL) ? fd[1] : out_fd) == ERR) {
            return ERR; 
        }
        /* parent */
        if (fd_in != in_fd) {
            close(fd_in);
        }
        if (curr->pipe != NULL) {
            close(fd[1]);
            fd_in = fd[0];
        }
        curr = curr->pipe;
    }
    
    printf("TEST!!! TEST!!! TEST!!! %d\n", pid_last);
    
    if (!in_backgrnd) {
        while (waitpid(pid_last, &status, 0) > 0); 
    }
    
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        return SUCC;
    else
        return ERR;
}

int exec_comm(tree t, int in_backgrnd, int in_pipe, int in_fd, int out_fd)
{
    int res = SUCC;
    if (t != NULL) {
        if (t->psubcmd == NULL)
            return exec_smpl_comm(t, in_backgrnd, in_pipe, in_fd, out_fd);
        else {
            int status;
            pid_t pid;
            pid = fork();
            if (pid == 0) { /* subshell */
                /* signal */
                if (in_backgrnd)
                    signal(SIGINT, SIG_IGN);
                else
                    signal(SIGINT, SIG_DFL);
                
                /* 0 -> /dev/null */
                if (in_backgrnd) {
                    int null_fd = open("/dev/null", O_RDONLY);
                    if (null_fd != -1) {
                        dup2(null_fd, 0); 
                        close(null_fd);
                    }
                }
                
                /* conveer */
                if (in_fd != 0) {
                    dup2(in_fd, 0);
                    close(in_fd);
                }
                if (out_fd != 1) {
                    dup2(out_fd, 1);
                    close(out_fd);
                }
                return exec_comm_list(t->psubcmd, in_backgrnd, in_pipe, in_fd, out_fd);
            } else if (pid > 0) { /* parent */
                /* conveer */
                if (in_fd != 0)
                    close(in_fd);
                if (out_fd != 1)
                    close(out_fd);
                
                switch (in_backgrnd) {
                case 0:
                    if (!in_pipe) {
                        wait(&status);
                        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
                            return SUCC;
                        else
                            return ERR;
                    } else {
                        add_backgrnd(pid);
                        pid_last = pid;
                        return SUCC;
                    }
                case 1:
                    add_backgrnd(pid);
                    if (!in_pipe)
                        print_in_backgrnd();
                    else
                        pid_last = pid;
                    return SUCC;
                }
            } else
            /* fork failure */
            return ERR;
        }
    }
    return res;
}

/* fork and wait here; return SUCC or ERR */
int exec_smpl_comm(tree t, int in_backgrnd, int in_pipe, int in_fd, int out_fd)
{   
    int res = SUCC;
    if (t != NULL) {
        /* check if built-in */
        word cmd = t->argv[0];
        if (!strcmp(cmd, "cd"))
            return exec_cd(t, in_backgrnd);
        else if (!strcmp(cmd, "pwd"))
            return exec_pwd(t, in_backgrnd);
        else if (!strcmp(cmd, "clear"))
            return exec_clear(t, in_backgrnd);
        
        int status;
        pid_t pid;
        pid = fork();
        if (pid == 0) { /* child */
            /* signal */
            if (in_backgrnd)
                signal(SIGINT, SIG_IGN);
            else
                signal(SIGINT, SIG_DFL);
            
            /* 0 -> /dev/null */
            if (in_backgrnd) {
                int null_fd = open("/dev/null", O_RDONLY);
                if (null_fd != -1) {
                    dup2(null_fd, 0); 
                    close(null_fd);
                }
            }
            
            /* conveer*/
            if (in_fd != 0) {
                dup2(in_fd, 0);
                close(in_fd);
            }
            if (out_fd != 1) {
                dup2(out_fd, 1);
                close(out_fd);
            }
            
            chng_io(t);
            execvp(t->argv[0], t->argv);
            /* HERE */
            exit(EXIT_FAILURE);
        } else if (pid > 0) { /* parent */
            /* conveer*/
            if (in_fd != 0)
                close(in_fd);
            if (out_fd != 1)
                close(out_fd);
            
            switch (in_backgrnd) {
            case 0:
                if (!in_pipe) {
                    wait(&status);
                    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
                        return SUCC;
                    else
                        return ERR;
                } else {
                    pid_last = pid;
                    return SUCC;
                }
            case 1:
                if (!in_pipe) {
                    add_backgrnd(pid);
                    print_in_backgrnd();
                } else
                    pid_last = pid;
                return SUCC;
            }
        } else
            /* fork failure */
            return ERR;
    }
    return res;
}
