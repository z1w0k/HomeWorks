#include "tree.h"
#include "list.h"
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

jmp_buf buf;

static void handler_signal(int sig) {
    (void)sig;
    printf("\n");
    siglongjmp(buf, 1);
}

int main() {
    signal(SIGINT, handler_signal);
    sigsetjmp(buf, 1);

    List lst = NULL;
    Tree *tr = NULL;
    char cwd[PATH_MAX];

    while (1) {
        if (!getcwd(cwd, sizeof(cwd))) strcpy(cwd, ".");
        printf("%s$ ", cwd);

        int eof = buildlist(&lst);
        if (eof) {
            printf("\n");
            break;
        }

        if (!lst) {
            clearlist(&lst);
            continue;
        }

        tr = create_tree(lst);
        if (tr) {
            if (tr->root) {
                print_tree(tr->root); // ← Обратите внимание: передаём root
            }
            clear_tree(&tr);
        }

        clearlist(&lst);
    }

    clearlist(&lst);
    if (tr) clear_tree(&tr);
    return 0;
}
