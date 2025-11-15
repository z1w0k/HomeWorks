#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "liststr.h"

static int isSpace(ListStr *list, int newSize) {
    if (newSize + 1 <= list->capacity) {
        return 0;
    }

    int newCapacity = list->capacity;
    if (newCapacity == 0) {
        newCapacity = LIST_BLOCK_SIZE;
    }
    while (newCapacity < newSize + 1) {
        newCapacity += LIST_BLOCK_SIZE;
    }

    char **tmp = realloc(list->data, (size_t)newCapacity * sizeof(char *));
    if (!tmp) {
        fprintf(stderr, "Allocation failed: unable to expand memory for word list\n");
        return -1;
    }

    list->data = tmp;
    list->capacity = newCapacity;
    return 0;
}

void initList(ListStr *list) {
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}

void clearList(ListStr *list) {
    if (!list) return;

    if (list->data) {
        for (int i = 0; i < list->size; ++i) {
            free(list->data[i]);
        }
        free(list->data);
    }

    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}

int addWord(ListStr *list, char *word) {
    if (!word) {
        return 0;
    }

    if (isSpace(list, list->size + 1) != 0) {
        free(word);
        return -1;
    }

    list->data[list->size++] = word;
    list->data[list->size] = NULL;
    return 0;
}

void finishList(ListStr *list) {
    if (!list->data) {
        list->data = (char **)malloc(sizeof(char *));
        if (!list->data) {
            fprintf(stderr, "Allocation failed: unable to allocate memory for empty list\n");
            list->size = 0;
            list->capacity = 0;
            return;
        }
        list->data[0] = NULL;
        list->capacity = 1;
        return;
    }

    if (list->size + 1 < list->capacity) {
        char **tmp = realloc(list->data,
                             (size_t)(list->size + 1) * sizeof(char *));
        if (tmp) {
            list->data = tmp;
            list->capacity = list->size + 1;
        }
    }

    list->data[list->size] = NULL;
}

void printList(const ListStr *list) {
    if (!list || !list->data) {
        printf("0\n");
        return;
    }

    printf("%d\n", list->size);
    for (int i = 0; i < list->size; ++i) {
        printf("%s\n", list->data[i]);
    }
}

void sortList(ListStr *list) {
    if (!list || list->size <= 1 || !list->data) {
        return;
    }

    int n = list->size;
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (strcmp(list->data[j], list->data[j + 1]) > 0) {
                char *temp = list->data[j];
                list->data[j] = list->data[j + 1];
                list->data[j + 1] = temp;
            }
        }
    }

    list->data[list->size] = NULL;
}
