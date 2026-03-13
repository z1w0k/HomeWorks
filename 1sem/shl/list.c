#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include "list.h"

#define BLOCK_SIZE 16
#define STEP_SIZE 16

extern jmp_buf begin;

static list lst;
static int sizelist;
static int curlist;

static word buf;
static int sizebuf;
static int curbuf;
static int maxlen;

int get_sym(void)
{
	return getchar();
}

int not_spec(int c) 
{
    return c != ';' && c != '<' && c != '(' && c != ')' && 
        c != '|' && c != '&' && c != '>' &&
        c != '#' && c != '"' && c != '\\';
}

/* Interface for list */

void init_list(void)
{
    sizelist = 0;
    curlist = 0;
    lst = NULL;
}

void free_list(void)
{
    sizelist = 0;
    curlist = 0;
    if (lst == NULL) return;
    
    int i;
    for (i = 0; lst[i] != NULL; i++)
        free(lst[i]);
    free(lst);
    lst = NULL;
}

void term_list(void)
{
    if (lst == NULL) return;

    list tmp;
    if (curlist > sizelist - 1) {
        tmp = (list)realloc(lst, (sizelist + 1)*sizeof(word));
        if (tmp != NULL)
            lst = tmp;
        else
            /* ERROR */;
    }

    lst[curlist] = NULL;

    tmp = (list)realloc(lst, (sizelist = curlist + 1)*sizeof(word));
    if (tmp != NULL)
        lst = tmp;
    else
        /* ERROR */;
}

void init_buf(void)
{
    buf = NULL;
    sizebuf = 0;
    curbuf = 0;
}

void add_sym(int c)
{
    word tmp;
    if (curbuf > sizebuf - 1) {
        tmp = (word)realloc(buf, (sizebuf += STEP_SIZE)*sizeof(char));
        if (tmp != NULL)
            buf = tmp;
        else
            /* ERROR */;
    }

    buf[curbuf++] = c;
}

void add_word(void)
{
    word tmp;
    if (curbuf > sizebuf - 1) {
        tmp = (word)realloc(buf, (sizebuf + 1)*sizeof(char));
        if (tmp != NULL)
            buf = tmp;
        else
            /* ERROR */;
    }

    buf[curbuf] = '\0';

    tmp = (word)realloc(buf, (sizebuf = curbuf + 1)*sizeof(char));
    if (tmp != NULL)
        buf = tmp;
    else
        /* ERROR */;
    if (sizebuf > maxlen)
        maxlen = sizebuf;
    
    list ptr;
    if (curlist > sizelist - 1) {
        ptr = (list)realloc(lst, (sizelist += STEP_SIZE)*sizeof(word));
        if (ptr != NULL)
            lst = ptr;
        else
            /* ERROR */;
    }
    
    lst[curlist++] = buf;
    
    if (!strcmp(buf, "exit")) {
		term_list();
		free_list();
		exit(0);
	}
}

/* Interface for lexical parsing // L-graph */

typedef void *(*vertex)(void);

static int c;
static int building;

static int freq;
static int freqspec; /* Incorrect */

void *start();
void *in_word();
void *pipe1();
void *aper1();
void *greater1();
void *in_comment();
void *in_str();
void *spec();
void *stop();
void *err();

void *start(void)
{
    if (c == ' ' || c == '\t') {
        c = get_sym();
        return start;
    } else if (c == EOF) {
		term_list();
		free_list();
		exit(0);
    } else if (c == '\n') {
        term_list(); /* list is formed */
        return stop;
	} else {
        init_buf();
        if (c == '\\') return in_word;
        if (c != '"' && c != '#')
			add_sym(c);
        char prev = c;
        c = get_sym();
        switch(prev) {
            case ';': case '<': case '(': case ')': return spec;
            case '|': return pipe1;
            case '&': return aper1;
            case '>': return greater1;
            case '"': return in_str;
            case '#': return in_comment;
            default: return in_word;
        }
    }
}

void *in_word(void)
{
    if (not_spec(c) && c != '\n' && c != ' ' && c != '\t' && c != EOF) {
        add_sym(c);
        c = get_sym();
        return in_word;
    }
    if (c == '\\') { /* Wrong for \$USER etc */
		c = get_sym();
		if (c != '\n' && c != ' ' && c != '\t' && c != EOF) {
			add_sym(c);
			c = get_sym();
			return in_word;
		}
	}
	add_word();
	freq += 1;
	return start;
}

void *pipe1(void)
{
    if (c == '|') {
        add_sym(c);
        c = get_sym();
        return spec;
    } else {
        add_word();
		freqspec += 1;
        return start;
    }
}

void *aper1(void)
{
    if (c == '&') {
        add_sym(c);
        c = get_sym();
        return spec;
    } else {
        add_word();
		freqspec += 1;
        return start;
    }
}

void *greater1(void)
{
    if (c == '>') {
        add_sym(c);
        c = get_sym();
        return spec;
    } else {
        add_word();
		freqspec += 1;
        return start;
    }
}

void *in_comment(void)
{
	if (c != '\n' && c != EOF) {
		c = get_sym();
		return in_comment;
	} else
		return start;
}

void *in_str(void)
{
	if (c != EOF && c != '\n' && c != '"' ) {
		add_sym(c);
		c = get_sym();
		return in_str;
	} else if (c == '"') {
		add_word();
		c = get_sym();
		return start;
	} else
		return err;
}

void *spec(void)
{
    add_word();
    freqspec += 1;
    return start;
}

void *stop(void)
{
    building = 0;
    return stop;
}

/* ERROR if str is invalid */
void *err(void)
{
	add_word();
	term_list();
	free_list();
	longjmp(begin, 1);
}

list build_list(void)
{
	building = 1;
    vertex v = start;
    c = get_sym();
    init_list();
        
    while (building)
        v = v();
    
    return lst;
}
