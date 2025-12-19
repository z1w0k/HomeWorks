#ifndef TOKENS_H
#define TOKENS_H

struct tokens {
    char **list;
    int size;
    int count;
};

int input_finished(void);
void add_token(struct tokens *t, char *str);
struct tokens *get_tokens(void);
void sort_tokens(struct tokens *t);
void print_tokens(struct tokens *t);
void free_tokens(struct tokens *t);

#endif
