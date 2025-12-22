#ifndef __LIST_H__
#define __LIST_H__

typedef struct list {
    char *word;
    struct list *next;
} *List;

int buildlist(List *lst);
void clearlist(List *lst);

#endif
