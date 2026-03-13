#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "tree.h"

static list lst;

void error(char *, char *)
{
	return;
}

void init_comm(tree t)
{
	t->argv = NULL;
	
	t->infile = NULL;
	t->outfile = NULL;
	t->append = 0;
	
	t->backgrnd = 0;
	
	t->psubcmd = NULL;
	t->pipe = NULL;
	t->next = NULL;
	
	t->type = NXT;
}

/* *lst != NULL */
int is_oper(void)
{
	return !strcmp(*lst, "&&") || !strcmp(*lst, "||");
}

/* *lst != NULL */
int is_next(void)
{
	return !strcmp(*lst, "&") || !strcmp(*lst, ";");
}

/* *lst != NULL */
int is_inout(void)
{
	return !strcmp(*lst, "<") || !strcmp(*lst, ">") || !strcmp(*lst, ">>");
}

/* *lst != NULL */
int is_out(void)
{
	return !strcmp(*lst, ">") || !strcmp(*lst, ">>");
}

/* *lst != NULL */
int spec_word(void)
{
	return !strcmp(*lst, "<") || !strcmp(*lst, ">") ||
		!strcmp(*lst, ">>") || !strcmp(*lst, "(") || !strcmp(*lst, ")") ||
		!strcmp(*lst, "|") || !strcmp(*lst, "&") || !strcmp(*lst, "&&") ||
		!strcmp(*lst, "||") || !strcmp(*lst, ";");
}

/* <, >, >> */
word in_out(tree t)
{
	/* t != NULL */
	word ptr = *lst++;
	if (*lst == NULL) {
		printf("ERROR IN IN_OUT\n");
		return NULL;
	}
	
	/* CHECK IF FILENAME */
	
	word tmp = (word)calloc(strlen(*lst) + 1, sizeof(char));
	if (tmp == NULL)
		/* MEMORY ERROR */;
	strcpy(tmp, *lst);
	if (!strcmp(ptr, "<"))
		t->infile = tmp;
	else
		t->outfile = tmp;
	if (!strcmp(ptr, ">>"))
		t->append = 1;
	lst++;
	
	return tmp;
}

tree comm_shell();
tree comm_list();
tree conv();
tree comm();
tree smpl_comm();

tree comm_shell(void)
{
	return comm_list();
}

tree comm_list(void)
{
	tree t = conv();
	if (t == NULL)
		return NULL;
	
	int flag_next = 0;
	tree tmp = t, next;
	while (*lst != NULL && (is_oper() || is_next())) {
		if (!strcmp(*lst, "&&"))
			tmp->type = AND;
		else if (!strcmp(*lst, "||"))
			tmp->type = OR;
		else {
			tmp->type = NXT;
			flag_next = 1;
		}
			
		if (!strcmp(*lst, "&"))
			tmp->backgrnd = 1;
		
		lst++;
		next = conv();
		tmp->next = next;
		if (next == NULL && flag_next)
			break;
		else if (next == NULL) {
			printf("ERROR IN COMM_LIST\n");
			return t;
		}
		tmp = tmp->next;
	}
	
	return t;
}

tree conv(void)
{
	tree t = comm();
	if (t == NULL)
		return NULL;
	
	tree tmp = t, next;
	while (*lst != NULL && !strcmp(*lst, "|")) {
		lst++;
		next = comm();
		if (next == NULL) {
			/* ERROR IN SYNT */
			printf("ERROR IN CONV\n");
			return t;
		}
		tmp->pipe = next;
		tmp = tmp->pipe;
	}
	
	return t;
}

tree comm(void)
{
	if (*lst != NULL && !strcmp(*lst, "(")) {
		lst++;
		tree tmp = comm_list();
		if (tmp == NULL) {
			printf("ERROR IN COMM 1\n");
			return NULL;
		}
		if (*lst != NULL && !strcmp(*lst, ")")) {
			tree t = (tree)calloc(1, sizeof(node));
			if (t == NULL)
				/* MEMORY ERROR */;
			init_comm(t);
			t->psubcmd = tmp;
			lst++;
			
			if (*lst != NULL && is_inout())
				if (in_out(t) == NULL) {
					printf("ERROR IN COMM 2\n");
					return t;
				}
			if (*lst != NULL && is_out())
				if (in_out(t) == NULL) {
					printf("ERROR IN COMM 3\n");
					return t;
				}
			
			return t;
		} else {
			printf("ERROR IN COMM 4\n");
			return tmp;
		}
	} else if (*lst != NULL)
		return smpl_comm();
	else
		return NULL;
}

tree smpl_comm(void)
{
	if (*lst == NULL || spec_word())
		return NULL;

	tree t = (tree)calloc(1, sizeof(node));
	if (t == NULL)
		/* MEMORY ERROR */;
	init_comm(t);
		
	list tmp = NULL;
	int i = 1;
	do {
		tmp = (list)realloc(tmp, ++i*sizeof(word));
		if (tmp == NULL)
			/* MEMORY ERROR */;
		word ptr = (word)calloc(strlen(*lst) + 1, sizeof(char));
		if (ptr == NULL)
			/* MEMORY ERROR */;
		strcpy(ptr, *lst++);
		tmp[i - 2] = ptr;
	} while (*lst != NULL && !spec_word());
	tmp[i - 1] = NULL;
	t->argv = tmp;
	
	if (*lst != NULL && is_inout())
		if (in_out(t) == NULL) {
			printf("ERROR IN SMPL_COMM 1\n");
			return t;
		}
	if (*lst != NULL && is_out())
		if (in_out(t) == NULL) {
			printf("ERROR IN SMPL_COMM 2\n");
			return t;
		}
	
	return t;
}

void fix_backgrnd(tree t)
{
	if (t != NULL) {
		if (t->backgrnd) {
			tree tmp = t->psubcmd;
			if (tmp != NULL)
				tmp->backgrnd = 1;
			tmp = t->pipe;
			if (tmp != NULL)
				tmp->backgrnd = 1;
		}
		fix_backgrnd(t->psubcmd);
		fix_backgrnd(t->pipe);
		fix_backgrnd(t->next);
	}
}

tree build_tree(list l)
{
	if (l == NULL) return NULL;
	
	lst = l;
	tree t = comm_shell();
	
	if (*lst != NULL)
		/* ERROR IN SYNT*/
		printf("ERROR IN BUILD_TREE\n");
	
	fix_backgrnd(t);
	
	return t;
}

void free_tree(tree t)
{
	if (t != NULL) {
		list tmp = t->argv;
		if (tmp != NULL)
			while (*tmp != NULL)
				free(*tmp++);
		free(t->argv);
		
		free(t->infile);
		free(t->outfile);
		
		free_tree(t->psubcmd);
		free_tree(t->pipe);
		free_tree(t->next);
		
		free(t);
	}
}
