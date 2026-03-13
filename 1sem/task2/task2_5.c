#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char* data;
    struct Node* next;
};

struct Node* createNode(const char* val) {
    struct Node* newNode = calloc(1, sizeof(struct Node));
    if (newNode == NULL) {
        printf("Not enough memory\n");
        return NULL;
    }

    newNode->data = malloc(strlen(val) + 1);
    if (newNode->data == NULL) {
        printf("Not enough memory for string\n");
        free(newNode);
        return NULL;
    }
    strcpy(newNode->data, val);
    newNode->next = NULL;
    return newNode;
}

void appendNode(struct Node** head, const char* val) {
    struct Node* newNode = createNode(val);
    if (newNode == NULL) return;

    if (*head == NULL) {
        *head = newNode;
    } else {
        struct Node* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

char* getLastWord(struct Node* head) {
    if (head == NULL) return NULL;

    struct Node* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    return current->data;
}

void removeMatchingWords(struct Node** head) {
    if (*head == NULL) return;

    char* lastWord = getLastWord(*head);
    if (lastWord == NULL) return;

    struct Node* current = *head;
    struct Node* prev = NULL;
    struct Node* lastNode = NULL;

    struct Node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    lastNode = temp;

    while (current != NULL) {
        if (strcmp(current->data, lastWord) == 0 && current != lastNode) {
            struct Node* toDelete = current;

            if (prev == NULL) {
                *head = current->next;
                current = current->next;
            } else {
                prev->next = current->next;
                current = current->next;
            }

            free(toDelete->data);
            free(toDelete);
        } else {
            prev = current;
            current = current->next;
        }
    }
}

void printList(struct Node* head) {
    struct Node* current = head;
    printf("[");
    while (current != NULL) {
        printf("%s", current->data);
        if (current->next != NULL) {
            printf(", ");
        }
        current = current->next;
    }
    printf("]");
    printf("\n");
}

void freeList(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        struct Node* next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
}


int main() {
    char word[100];
    struct Node* wordList = NULL;

    while (scanf("%99s", word) != EOF) {
        appendNode(&wordList, word);
    }

    if (wordList != NULL) {
        removeMatchingWords(&wordList);
        printList(wordList);
        freeList(wordList);
    }

    return 0;
}
