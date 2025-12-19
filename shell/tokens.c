#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "tokens.h"

int eof_flag = 0;

int input_finished(void)
{
    return eof_flag;
}

void add_token(struct tokens *t, char *str)
{
    if (t->size == t->count){
        t->size += 10;
        t->list = realloc(t->list, t->size * sizeof(char *));
    }
    t->list[t->count] = str;
    t->count++;
}

int get_char(void)
{
    static char buffer[1024];
    static int read_count = 0;
    static int given_count = 0;
    if (given_count == read_count){
        read_count = read(0, buffer, sizeof(buffer));
        if (read_count < 1){
            eof_flag = 1;
            return EOF;
        }
        given_count = 0;
    }
    return buffer[given_count++];
}

char *copy_string(char *src, int len)
{
    char *dst = malloc(len + 1);
    int i;
    for (i = 0; i < len; i++){
        dst[i] = src[i];
    }
    dst[len] = '\0';
    return dst;
}

struct tokens *get_tokens(void)
{
    int ch;
    int spec_flag = 0;
    int quote_flag = 0;
    int buf_size = 10;
    int buf_used = 0;
    char *buffer = malloc(buf_size);
    struct tokens *t = malloc(sizeof(struct tokens));
    memset(t, 0, sizeof(struct tokens));
    while ((ch = get_char()) != EOF){
        if (ch == '\n')
            break;
        if (ch == '"'){
            if (quote_flag == 1){
                add_token(t, copy_string(buffer, buf_used));
                buf_used = 0;
                quote_flag = 0;
            }
            else {
                quote_flag = 1;
            }
            continue;
        }
        if (ch != '|' && ch != '&' && ch != '>' && spec_flag && !quote_flag){
            add_token(t, copy_string(buffer, buf_used));
            buf_used = 0;
            spec_flag = 0;
        }
        if ((ch == ' ' || ch == '\t' || ch == '\n' ||
            ch == '|' || ch == '&'  || ch == ';'  ||
            ch == '>' || ch == '<'  || ch == '('  || ch == ')') && !quote_flag){
            if (buf_used > 0 && !spec_flag){
                add_token(t, copy_string(buffer, buf_used));
                buf_used = 0;
            }
            switch(ch){
                case '(': case ')': case ';': case '<':
                    buffer[buf_used++] = ch;
                    add_token(t, copy_string(buffer, buf_used));
                    buf_used = 0;
                    break;
                case '|': case '&': case '>': 
                    if (spec_flag){
                        if (buffer[buf_used - 1] == ch){
                            buffer[buf_used++] = ch;
                            spec_flag = 0;
                        }
                        add_token(t, copy_string(buffer, buf_used));
                        buf_used = 0;
                        if (spec_flag) {
                            buffer[buf_used++] = ch;
                        }
                    }
                    else {
                        spec_flag = 1;
                        buffer[buf_used++] = ch;
                    }
            }
            continue;
        }
        if (buf_used == buf_size){
            buf_size = buf_size * 2;
            char *new_buf = malloc(buf_size);
            int i;
            for(i = 0; i < buf_used; i++){
                new_buf[i] = buffer[i];
            }
            free(buffer);
            buffer = new_buf;
        }
        buffer[buf_used++] = ch;
    }
    if (buf_used > 0){
        add_token(t, copy_string(buffer, buf_used));
    }
    free(buffer);
    add_token(t, NULL);
    t->count--;
    return t;
}

void sort_tokens(struct tokens *t)
{
    char **arr = t->list;
    int n = t->count;
    int i, j;
    char *tmp;
    for (i = 0; i < n; i++){
        for (j = 0; j < n - 1; j++){
            if (strcmp(arr[j], arr[j + 1]) > 0){
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

void print_tokens(struct tokens *t)
{
    int i;
    printf("%d\n", t->count);
    for (i = 0; i < t->count; i++)
        printf("%s\n", t->list[i]);
}

void free_tokens(struct tokens *t)
{
    int i;
    for (i = 0; i < t->count; i++)
        free(t->list[i]);
    free(t->list);
    free(t);
}
