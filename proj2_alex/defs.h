#ifndef DEFS_H
#define DEFS_H

#include "csapp.h"
#include <stdbool.h>
#define MAXARGS 128
#define MAXPIPE 101

typedef struct{
    char** argv; //array of argument strings.
    int** pipe_fds; //array of file descriptor arrays. must dynamically allocate
    bool has_prev_input; // whether the process has a previous input in the pipe
    int bg; //whether there is a background process or not
    
} command_t;

char* first_args[MAXARGS];
char* rest_args[MAXARGS];

void log_force(char* log);
bool is_argv_pipe(char** argv);
void error_quit(char* msg, const char* func_name);
void print_args_with_str(char* str, char** args);
int num_of_pipes(char** argv);

#endif 
