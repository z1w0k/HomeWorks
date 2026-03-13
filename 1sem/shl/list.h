#ifndef LIST_H
#define LIST_H

typedef char *word;
typedef char **list;

list build_list(void);
void free_list(void);
int print_list(list);
void substitute(list);

#endif
