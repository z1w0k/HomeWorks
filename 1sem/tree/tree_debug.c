#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <signal.h>
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
    tree *tr = NULL;
    int sizelist, ifeof;
    int isthere;
    
    char cwd[PATH_MAX];
    
    while (1) {
        isthere = 0;
        
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            error("Невозможно получить текущий путь!\0");
            strcpy(cwd, ".");
        }
        
        printf("%s$ ", cwd);
        
        ifeof = buildlist(&lst, &sizelist, &isthere);
        
        if (!isthere) {
            if (ifeof) {
                printf("\n");
                break;
            }
            
            if (lst == NULL) {
                fflush(stdin);
                clearlist(&lst);
                cleanup_lexer();
                continue;
            }
            
            changelist(lst, sizelist);
            tr = create_tree(lst);
            
            if (tr != NULL) {
                printf("\n=== PARSED COMMAND TREE ===\n");
                print_tree(tr, 0);
                printf("=== END TREE ===\n\n");
                
                clear_tree(&tr);
                tr = NULL;
            }
            
        } else {
            fprintf(stderr, "Error: parentheses not supported\n");
        }
        
        clearlist(&lst);
        cleanup_lexer();
        lst = NULL;
    }
    
    clearlist(&lst);
    cleanup_lexer();
    if (tr != NULL) {
        clear_tree(&tr);
    }
    
    return 0;
}
