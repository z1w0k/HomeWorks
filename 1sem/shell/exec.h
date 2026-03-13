#ifndef __EXEC_H__
#define __EXEC_H__

#include "tree.h"
#include <stdbool.h>
#include <signal.h> 

extern volatile int flag_exit;

void execute_tree(Tree *tr);

#endif
