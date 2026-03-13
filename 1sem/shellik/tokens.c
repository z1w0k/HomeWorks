#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "tokens.h"

int eof_flag = 0;
int comment_flag = 0;

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
    return (unsigned char)buffer[given_count++];
}

char *copy_string(char *src, int len)
{
    if (len <= 0) return NULL;
    
    char *dst = malloc(len + 1);
    if (!dst) return NULL;
    
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
    int spec_flag = 0, quote_flag = 0, dquote_flag = 0;
    int buf_size = 10, buf_used = 0;
    char *buffer = malloc(buf_size);
    struct tokens *t = malloc(sizeof(struct tokens));
    
    memset(t, 0, sizeof(struct tokens));
    comment_flag = 0;
    
    while ((ch = get_char()) != EOF){
        if (ch == '#' && !quote_flag && !dquote_flag && !spec_flag && buf_used == 0) {
            comment_flag = 1;
            while ((ch = get_char()) != EOF && ch != '\n')
                ;
            if (ch == '\n') break;
            continue;
        }
        
        if (comment_flag && ch != '\n') {
            continue;
        }
        
        if (ch == '\n') {
            if (quote_flag || dquote_flag) {
                if (buf_used == buf_size) {
                    buf_size *= 2;
                    char *new_buf = malloc(buf_size);
                    for(int i = 0; i < buf_used; i++) new_buf[i] = buffer[i];
                    free(buffer);
                    buffer = new_buf;
                }
                buffer[buf_used++] = ch;
            } else {
                break;
            }
        }
        
        if (ch == '\\' && !quote_flag) {
            int next_ch = get_char();
            if (next_ch == EOF) {
                eof_flag = 1;
                break;
            }
            
            if (buf_used == buf_size) {
                buf_size *= 2;
                char *new_buf = malloc(buf_size);
                for(int i = 0; i < buf_used; i++) new_buf[i] = buffer[i];
                free(buffer);
                buffer = new_buf;
            }
            
            buffer[buf_used++] = next_ch;
            continue;
        }
        
        if (ch == '\'' && !dquote_flag && !quote_flag) {
            quote_flag = 1;
            continue;
        } else if (ch == '\'' && quote_flag) {
            quote_flag = 0;
            continue;
        }
        
        if (ch == '"' && !quote_flag && !dquote_flag) {
            dquote_flag = 1;
            continue;
        } else if (ch == '"' && dquote_flag) {
            dquote_flag = 0;
            continue;
        }
        
        if (quote_flag) {
            if (buf_used == buf_size) {
                buf_size *= 2;
                char *new_buf = malloc(buf_size);
                for(int i = 0; i < buf_used; i++) new_buf[i] = buffer[i];
                free(buffer);
                buffer = new_buf;
            }
            buffer[buf_used++] = ch;
            continue;
        }
        
        if ((ch == '|' || ch == '&' || ch == ';' || ch == '<' || ch == '>' || 
             ch == '(' || ch == ')' || ch == ' ' || ch == '\t') && 
            !dquote_flag && !quote_flag) {
            
            if (buf_used > 0) {
                add_token(t, copy_string(buffer, buf_used));
                buf_used = 0;
            }
            
            if (ch == '|' || ch == '&' || ch == '>') {
                if (spec_flag) {
                    if (buf_used < buf_size) {
                        buffer[buf_used++] = ch;
                    }
                    add_token(t, copy_string(buffer, buf_used));
                    buf_used = 0;
                    spec_flag = 0;
                } else {
                    spec_flag = 1;
                    buf_used = 0;
                    if (buf_size < 2) {
                        buf_size = 2;
                        free(buffer);
                        buffer = malloc(buf_size);
                    }
                    buffer[buf_used++] = ch;
                }
                continue;
            } else if (spec_flag) {
                add_token(t, copy_string(buffer, buf_used));
                buf_used = 0;
                spec_flag = 0;
            }
            
            if (ch == ';' || ch == '<' || ch == '(' || ch == ')') {
                char special[2] = {ch, '\0'};
                add_token(t, strdup(special));
                continue;
            }
            
            if (ch == ' ' || ch == '\t') {
                continue;
            }
        }
        
        if (spec_flag && !dquote_flag && !quote_flag && 
            ch != '|' && ch != '&' && ch != '>') {
            add_token(t, copy_string(buffer, buf_used));
            buf_used = 0;
            spec_flag = 0;
        }
        
        if (buf_used == buf_size) {
            buf_size *= 2;
            char *new_buf = malloc(buf_size);
            for(int i = 0; i < buf_used; i++) new_buf[i] = buffer[i];
            free(buffer);
            buffer = new_buf;
        }
        
        buffer[buf_used++] = ch;
    }
    
    if (buf_used > 0 && !spec_flag) {
        add_token(t, copy_string(buffer, buf_used));
    } else if (spec_flag) {
        add_token(t, copy_string(buffer, buf_used));
    }
    
    free(buffer);
    
    if (t->count > 0) {
        add_token(t, NULL);
        t->count--;
    }
    
    return t;
}

void sort_tokens(struct tokens *t)
{
    if (!t || t->count <= 1) return;
    
    char **arr = t->list;
    int n = t->count;
    int i, j;
    char *tmp;
    
    for (i = 0; i < n - 1; i++){
        for (j = 0; j < n - i - 1; j++){
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
    if (!t) {
        printf("No tokens\n");
        return;
    }
    
    printf("%d\n", t->count);
    for (int i = 0; i < t->count; i++)
        printf("%s\n", t->list[i] ? t->list[i] : "(null)");
}

void free_tokens(struct tokens *t)
{
    if (!t) return;
    
    int i;
    for (i = 0; i < t->count; i++)
        free(t->list[i]);
    
    free(t->list);
    free(t);
}
