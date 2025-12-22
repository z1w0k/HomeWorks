#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <pwd.h>

#define MAX_LINE 1024

int buildlist(List *lst) {
    char buffer[MAX_LINE];
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return 1;
    }

    *lst = NULL;
    List *tail = lst;

    char *p = buffer;
    while (*p) {
        while (isspace((unsigned char)*p)) p++;
        if (*p == '\0' || *p == '\n') break;

        char *start = p;
        if (*p == '|' || *p == '<' || *p == '>' || *p == '&' || *p == ';') {
            if (*p == '>' && *(p + 1) == '>') {
                p += 2;
                char *dup = strdup(">>");
                List node = malloc(sizeof(struct list));
                node->word = dup;
                node->next = NULL;
                *tail = node;
                tail = &node->next;
            } else if (*p == '&' && *(p + 1) == '&') {
                p += 2;
                char *dup = strdup("&&");
                List node = malloc(sizeof(struct list));
                node->word = dup;
                node->next = NULL;
                *tail = node;
                tail = &node->next;
            } else if (*p == '|' && *(p + 1) == '|') {
                p += 2;
                char *dup = strdup("||");
                List node = malloc(sizeof(struct list));
                node->word = dup;
                node->next = NULL;
                *tail = node;
                tail = &node->next;
            } else {
                char sym[2] = {*p, '\0'};
                char *dup = strdup(sym);
                List node = malloc(sizeof(struct list));
                node->word = dup;
                node->next = NULL;
                *tail = node;
                tail = &node->next;
                p++;
            }
        } else {
            if (*p == '$') {
                p++;
                if (strncmp(p, "HOME", 4) == 0 && (p[4] == '\0' || isspace(p[4]) || p[4] == '|' || p[4] == '<' || p[4] == '>' || p[4] == '&' || p[4] == ';' || p[4] == '\n')) {
                    p += 4;
                    char *val = getenv("HOME");
                    if (!val) val = "/";
                    char *dup = strdup(val);
                    List node = malloc(sizeof(struct list));
                    node->word = dup;
                    node->next = NULL;
                    *tail = node;
                    tail = &node->next;
                    continue;
                } else if (strncmp(p, "SHELL", 5) == 0 && (p[5] == '\0' || isspace(p[5]) || p[5] == '|' || p[5] == '<' || p[5] == '>' || p[5] == '&' || p[5] == ';' || p[5] == '\n')) {
                    p += 5;
                    char *val = getenv("SHELL");
                    if (!val) val = "/bin/sh";
                    char *dup = strdup(val);
                    List node = malloc(sizeof(struct list));
                    node->word = dup;
                    node->next = NULL;
                    *tail = node;
                    tail = &node->next;
                    continue;
                } else if (strncmp(p, "USER", 4) == 0 && (p[4] == '\0' || isspace(p[4]) || p[4] == '|' || p[4] == '<' || p[4] == '>' || p[4] == '&' || p[4] == ';' || p[4] == '\n')) {
                    p += 4;
                    char *val = getlogin();
                    if (!val) {
                        struct passwd *pw = getpwuid(geteuid());
                        if (pw) val = pw->pw_name;
                        else val = "unknown";
                    }
                    char *dup = strdup(val);
                    List node = malloc(sizeof(struct list));
                    node->word = dup;
                    node->next = NULL;
                    *tail = node;
                    tail = &node->next;
                    continue;
                } else if (strncmp(p, "EUID", 4) == 0 && (p[4] == '\0' || isspace(p[4]) || p[4] == '|' || p[4] == '<' || p[4] == '>' || p[4] == '&' || p[4] == ';' || p[4] == '\n')) {
                    p += 4;
                    char buf[32];
                    snprintf(buf, sizeof(buf), "%u", geteuid());
                    char *dup = strdup(buf);
                    List node = malloc(sizeof(struct list));
                    node->word = dup;
                    node->next = NULL;
                    *tail = node;
                    tail = &node->next;
                    continue;
                }
                p--;
            }

            while (*p && !isspace((unsigned char)*p) &&
                   *p != '|' && *p != '<' && *p != '>' && *p != '&' && *p != ';') {
                if (*p == '\'' || *p == '"') {
                    char quote = *p;
                    p++;
                    while (*p && *p != quote) p++;
                    if (*p) p++;
                } else {
                    p++;
                }
            }
            char *word = strndup(start, p - start);
            List node = malloc(sizeof(struct list));
            node->word = word;
            node->next = NULL;
            *tail = node;
            tail = &node->next;
        }
    }
    return 0;
}

void clearlist(List *lst) {
    while (*lst) {
        List tmp = *lst;
        *lst = (*lst)->next;
        free(tmp->word);
        free(tmp);
    }
}
