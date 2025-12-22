#ifndef __EXEC_H__
#define __EXEC_H__

#include "tree.h"
#include <stdbool.h>
#include <signal.h>  // <-- ДОБАВЛЕНО для sig_atomic_t

extern volatile sig_atomic_t flag_exit;

void execute_tree(Tree *tr);

#endif
