#ifndef LISTSTR_H
#define LISTSTR_H

#define LIST_BLOCK_SIZE 16

typedef struct {
    char **data;
    int size;
    int capacity;
} ListStr;

void initList(ListStr *list);
void clearList(ListStr *list);
int addWord(ListStr *list, char *word);
void finishList(ListStr *list);
void printList(const ListStr *list);
void sortList(ListStr *list);

#endif
