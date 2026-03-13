#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include "variables.h"

char *expand_variable(const char *token)
{
    if (!token || token[0] != '$') return NULL;
    
    if (strcmp(token, "$HOME") == 0) {
        char *home = getenv("HOME");
        if (!home) {
            struct passwd *pw = getpwuid(getuid());
            home = pw ? pw->pw_dir : "/";
        }
        return strdup(home);
    }
    else if (strcmp(token, "$SHELL") == 0) {
        char *shell = getenv("SHELL");
        return shell ? strdup(shell) : strdup("/bin/sh");
    }
    else if (strcmp(token, "$USER") == 0) {
        char *user = getenv("USER");
        if (!user) {
            struct passwd *pw = getpwuid(getuid());
            user = pw ? pw->pw_name : "unknown";
        }
        return strdup(user);
    }
    else if (strcmp(token, "$EUID") == 0) {
        char euid_str[20];
        snprintf(euid_str, sizeof(euid_str), "%d", geteuid());
        return strdup(euid_str);
    }
    
    if (strlen(token) > 1) {
        char *var_name = strdup(token + 1);
        char *value = getenv(var_name);
        free(var_name);
        if (value) {
            return strdup(value);
        }
    }
    
    return NULL;
}

void expand_command_args(char **args)
{
    if (!args) return;
    
    for (int i = 0; args[i]; i++) {
        char *arg = args[i];
        if (arg[0] == '$' && strlen(arg) > 1) {
            char *expanded = expand_variable(arg);
            if (expanded) {
                args[i] = expanded;
            }
        }
    }
}
