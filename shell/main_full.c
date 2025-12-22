#include "exec.h"
#include "list.h"
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>

static jmp_buf jmpbuf;
static Tree *global_tr = NULL;
static List global_lst = NULL;

static void cleanup_on_exit(void) {
    if (global_tr) clear_tree(&global_tr);
    if (global_lst) clearlist(&global_lst);
}

static void sigint_handler(int sig) {
    (void)sig;
    write(STDERR_FILENO, "\n", 1);
    cleanup_on_exit();
    siglongjmp(jmpbuf, 1);
}

int main() {
    atexit(cleanup_on_exit);
    signal(SIGINT, sigint_handler);
    sigsetjmp(jmpbuf, 1);

    char cwd[PATH_MAX];
    Tree *tr = NULL;
    List lst = NULL;

    while (!flag_exit) {
        if (!getcwd(cwd, sizeof(cwd))) strcpy(cwd, ".");
        printf("%s$ ", cwd);
        fflush(stdout);

        int eof = buildlist(&lst);
        if (eof) break;

        global_lst = lst;
        tr = create_tree(lst);
        if (tr) {
            global_tr = tr;
            execute_tree(tr);
            clear_tree(&tr);
            global_tr = NULL;
        }
        clearlist(&lst);
        global_lst = NULL;
    }

    printf("\n");
    return 0;
}
