
#ifndef EXECUTOR_H
#define EXECUTOR_H
// AST
#include "../parser/node.h"

char *find_executable_path(char *file);
int do_exec(int argc, char **argv);
int execute_command(node_s *node); // fork


#endif
