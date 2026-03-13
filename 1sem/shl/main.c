#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <setjmp.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <signal.h>

#include "list.h"
#include "tree.h"
#include "exec.h"

#define SHIFT 10

jmp_buf begin;

b_list backgrnd;

void handler(int);
void invite();

int main(int argc, char **argv)
{
	signal(SIGINT, handler);
	
	list l;
	tree t;
	while (1) {
			setjmp(begin);
			invite();
			l = build_list();
			substitute(l);
			t = build_tree(l);
			//print_tree(t, SHIFT);
			exec_comm_shell(t);
			clean_zombie(backgrnd);
			free_tree(t);
			free_list();
      }
}

void handler(int status)
{
    signal(SIGINT, handler);
}

void invite(void)
{
	printf("%s", "\x1B[01;36m");
	char s[100]; 
	gethostname(s, 100);
	printf("%s@%s", getenv("USER"), s);
	printf("%s", "\x1B[0m");
	printf(":");
	printf("%s", "\x1B[01;34m");
	getcwd(s, 100);
	printf("%s", s);
	printf("%s", "\x1B[0m");
	printf("$ ");	
}
