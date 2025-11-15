#include <stdio.h>
#include <stdlib.h>
#include "liststr.h"

#define INPUT_BLOCK_SIZE 64
#define BUF_BLOCK_SIZE 32

int currentChar;
char inputBlock[INPUT_BLOCK_SIZE];
int inputPos = 0;
int inputLen = 0;
int eofReached = 0;
char *buf = NULL;
int bufSize = 0;
int bufPos = 0;

typedef enum {
    Start, Word, Greater, Greater2, 
    Ampersand, Ampersand2, Pipe, Pipe2, 
    Newline, Stop
} vertex;

int getSym(void);
void nullBuf(void);
int addSym(int ch);
int addWordFromBuf(ListStr *list);
int isSimpleChar(int ch);

static vertex start(ListStr *list);
static vertex word(ListStr *list);
static vertex greater(ListStr *list);
static vertex greater2(ListStr *list);
static vertex ampersand(ListStr *list);
static vertex ampersand2(ListStr *list);
static vertex pipe(ListStr *list);
static vertex pipe2(ListStr *list);
static vertex newline(ListStr *list);

int main(void) {
    ListStr list;
    initList(&list);
    nullBuf();
    currentChar = getSym();
    vertex v = Start;

    while (1) {
        switch (v) {
            case Start: v = start(&list); break;
            case Word: v = word(&list); break;
            case Greater: v = greater(&list); break;
            case Greater2: v = greater2(&list); break;
            case Ampersand: v = ampersand(&list); break;
            case Ampersand2: v = ampersand2(&list); break;
            case Pipe: v = pipe(&list); break;
            case Pipe2: v = pipe2(&list); break;
            case Newline: v = newline(&list); break;
            case Stop: clearList(&list); return 0;
        }
    }
}

int getSym(void) {
    if (eofReached) return EOF;
    if (inputPos >= inputLen) {
        inputPos = 0;
        inputLen = 0;
        int r = fscanf(stdin, "%64c", inputBlock);
        if (r == 1) {
            inputLen = INPUT_BLOCK_SIZE;
        } else {
            int ch;
            while (inputLen < INPUT_BLOCK_SIZE && (ch = fgetc(stdin)) != EOF) {
                inputBlock[inputLen++] = (char)ch;
            }
            if (inputLen == 0) {
                eofReached = 1;
                return EOF;
            }
        }
    }
    return (unsigned char)inputBlock[inputPos++];
}

void nullBuf(void) {
    buf = NULL;
    bufSize = 0;
    bufPos = 0;
}

int addSym(int ch) {
    if (bufPos + 1 > bufSize) {
        int newSize = bufSize + BUF_BLOCK_SIZE;
        char *tmp = realloc(buf, (size_t)newSize);
        if (!tmp) {
            fprintf(stderr, "Allocation failed: unable to allocate memory for word\n");
            free(buf);
            buf = NULL;
            bufSize = 0;
            bufPos = 0;
            return -1;
        }
        buf = tmp;
        bufSize = newSize;
    }
    buf[bufPos++] = (char)ch;
    return 0;
}

int addWordFromBuf(ListStr *list) {
    if (!buf || bufPos == 0) return 0;
    if (bufPos + 1 > bufSize) {
        int newSize = bufPos + 1;
        char *tmp = realloc(buf, (size_t)newSize);
        if (!tmp) {
            fprintf(stderr, "Allocation failed: unable to finalize word (no memory)\n");
            free(buf);
            buf = NULL;
            bufSize = 0;
            bufPos = 0;
            return -1;
        }
        buf = tmp;
        bufSize = newSize;
    }
    buf[bufPos] = '\0';
    char *word = realloc(buf, (size_t)(bufPos + 1));
    if (!word) word = buf;
    if (addWord(list, word) != 0) {
        buf = NULL;
        bufSize = 0;
        bufPos = 0;
        return -1;
    }
    buf = NULL;
    bufSize = 0;
    bufPos = 0;
    return 0;
}

int isSimpleChar(int ch) {
    return ((ch >= '0' && ch <= '9') ||
            (ch >= 'A' && ch <= 'Z') ||
            (ch >= 'a' && ch <= 'z') ||
            ch == '$' || ch == '_' || ch == '/' || ch == '.');
}

static vertex start(ListStr *list) {
    while (currentChar == ' ' || currentChar == '\t') {
        currentChar = getSym();
    }
    if (currentChar == EOF) {
        if (bufPos > 0) {
            if (addWordFromBuf(list) != 0) {
                clearList(list);
                return Stop;
            }
        }
        if (list->size > 0) {
            finishList(list);
            printList(list);
            sortList(list);
            printList(list);
            clearList(list);
        }
        return Stop;
    }
    if (currentChar == '\n') return Newline;
    if (isSimpleChar(currentChar)) {
        nullBuf();
        if (addSym(currentChar) != 0) {
            clearList(list);
            return Stop;
        }
        currentChar = getSym();
        return Word;
    }
    if (currentChar == '>') {
        nullBuf();
        if (addSym(currentChar) != 0) {
            clearList(list);
            return Stop;
        }
        currentChar = getSym();
        return Greater;
    }
    if (currentChar == '&') {
        nullBuf();
        if (addSym(currentChar) != 0) {
            clearList(list);
            return Stop;
        }
        currentChar = getSym();
        return Ampersand;
    }
    if (currentChar == '|') {
        nullBuf();
        if (addSym(currentChar) != 0) {
            clearList(list);
            return Stop;
        }
        currentChar = getSym();
        return Pipe;
    }
    if (currentChar == ';' || currentChar == '<' ||
        currentChar == '(' || currentChar == ')') {
        nullBuf();
        if (addSym(currentChar) != 0 || addWordFromBuf(list) != 0) {
            clearList(list);
            return Stop;
        }
        currentChar = getSym();
        return Start;
    }
    currentChar = getSym();
    return Start;
}

static vertex word(ListStr *list) {
    if (currentChar == EOF || currentChar == '\n' ||
        currentChar == ' ' || currentChar == '\t' ||
        currentChar == '|' || currentChar == '&' ||
        currentChar == ';' || currentChar == '>' ||
        currentChar == '<' || currentChar == '(' ||
        currentChar == ')') {
        if (addWordFromBuf(list) != 0) {
            clearList(list);
            return Stop;
        }
        return Start;
    }
    if (!isSimpleChar(currentChar)) {
        if (addWordFromBuf(list) != 0) {
            clearList(list);
            return Stop;
        }
        currentChar = getSym();
        return Start;
    }
    if (addSym(currentChar) != 0) {
        clearList(list);
        return Stop;
    }
    currentChar = getSym();
    return Word;
}

static vertex greater(ListStr *list) {
    if (currentChar == '>') {
        if (addSym(currentChar) != 0) {
            clearList(list);
            return Stop;
        }
        currentChar = getSym();
        return Greater2;
    }
    if (addWordFromBuf(list) != 0) {
        clearList(list);
        return Stop;
    }
    return Start;
}

static vertex greater2(ListStr *list) {
    if (addWordFromBuf(list) != 0) {
        clearList(list);
        return Stop;
    }
    return Start;
}

static vertex ampersand(ListStr *list) {
    if (currentChar == '&') {
        if (addSym(currentChar) != 0) {
            clearList(list);
            return Stop;
        }
        currentChar = getSym();
        return Ampersand2;
    }
    if (addWordFromBuf(list) != 0) {
        clearList(list);
        return Stop;
    }
    return Start;
}

static vertex ampersand2(ListStr *list) {
    if (addWordFromBuf(list) != 0) {
        clearList(list);
        return Stop;
    }
    return Start;
}

static vertex pipe(ListStr *list) {
    if (currentChar == '|') {
        if (addSym(currentChar) != 0) {
            clearList(list);
            return Stop;
        }
        currentChar = getSym();
        return Pipe2;
    }
    if (addWordFromBuf(list) != 0) {
        clearList(list);
        return Stop;
    }
    return Start;
}

static vertex pipe2(ListStr *list) {
    if (addWordFromBuf(list) != 0) {
        clearList(list);
        return Stop;
    }
    return Start;
}

static vertex newline(ListStr *list) {
    if (bufPos > 0) {
        if (addWordFromBuf(list) != 0) {
            clearList(list);
            return Stop;
        }
    }
    if (list->size > 0) {
        finishList(list);
        printList(list);
        sortList(list);
        printList(list);
        clearList(list);
    }
    nullBuf();
    currentChar = getSym();
    return Start;
}
