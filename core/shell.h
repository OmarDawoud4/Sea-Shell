
#ifndef SHELL_H
#define SHELL_H

void print_prompt_dollar();
void print_prompt_gt();
char *read_cmd();



#include "../input/input_stream.h"
int parse_and_execute(input_stream_s *input_stream);

void initsh(void); 
// remove that cuffs
int dump(int argc, char **argv);

struct builtin_s
{
    char *name;   
    int (*func)(int argc, char **argv); 
    
};

extern struct builtin_s builtins[];

extern int builtins_count;

#endif