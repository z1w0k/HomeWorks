#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "list.h"

#define PATH_SIZE 64
#define PROC_SIZE 32

static char home[] = "$HOME";
static char shell[] = "$SHELL";
static char user[] = "$USER";
static char euid[] = "$EUID";

int print_list(list lst)
{
    int size = 0;
    
    if (lst == NULL) return size;
    
    while (*lst != NULL) {
		printf("%s\n", *lst++);
		size++;
	}
    
    return size;
}

void substitute_name(list lst)
{
	char *env = getenv(*lst + 1);
	int size_env = strlen(env) + 1;
	
	word tmp = (word)calloc(size_env, sizeof(char));
	if (tmp != NULL) {
		strncpy(tmp, env, size_env);
		tmp[size_env - 1] = '\0';
		free(*lst);
		*lst = tmp;
	} else
		/* ERROR */;	
}

void substitute_shell(list lst)
{
	word tmp = (word)calloc(PATH_SIZE + 1, sizeof(char));
	if (tmp != NULL) {
		char proc_path[PROC_SIZE];
		sprintf(proc_path, "/proc/%d/exe", getpid());
		int bytes = readlink(proc_path, tmp, PATH_SIZE);
		tmp[bytes] = '\0';
		free(*lst);
		*lst = tmp;
	} else
		/* ERROR */;
}

void substitute_user(list lst)
{
	char *usr = getlogin();
	int size_usr = strlen(usr) + 1;
	
	word tmp = (word)calloc(size_usr, sizeof(char));
	if (tmp != NULL) {
		strncpy(tmp, usr, size_usr);
		tmp[size_usr - 1] = '\0';
		free(*lst);
		*lst = tmp;
	} else
		/* ERROR */;
}

void substitute_euid(list lst)
{
	unsigned id, tmp_id;
	tmp_id = id = geteuid();
	int digits = 0;
	while (tmp_id > 0) {
		tmp_id /= 10;
		digits++;
	}
	
	word tmp = (word)calloc(++digits, sizeof(char));
	if (tmp != NULL) {
		snprintf(tmp, digits, "%d", id);
		free(*lst);
		*lst = tmp;
	} else
		/* ERROR */;
}

void substitute(list lst) /* Wrong for "$USER $HOME etc" */
{
	if (lst == NULL) return;
	
	while (*lst != NULL) {
		if (!strncmp(*lst, home, sizeof(home))) {
			substitute_name(lst);
		} else if (!strncmp(*lst, shell, sizeof(shell))) {
			substitute_shell(lst);
		} else if (!strncmp(*lst, user, sizeof(user))) {
			substitute_user(lst);
		} else if (!strncmp(*lst, euid, sizeof(euid))) {
			substitute_euid(lst);
		}
		lst++;
	}
}

#if 0
void sort(list lst)
{
    if (lst == NULL) return;
}
#endif
