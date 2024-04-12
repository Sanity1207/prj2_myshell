#ifndef DEFS_H
#define DEFS_H

#include "csapp.h"
#include <stdbool.h>
#define MAXARGS 128

typedef struct{
    char** argv; //array of argument strings.
    int* pipe_fd; //read,write end file descriptors for the pipe
    bool has_prev_input; // whether the process has a previous input in the pipe
    int bg; //whether there is a background process or not
} command_t;

char* first_args[MAXARGS];
char* rest_args[MAXARGS];

void log_force(char* log);
bool is_argv_pipe(char** argv);
void error_quit(char* msg, const char* func_name);

#endif 
