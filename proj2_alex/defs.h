#ifndef DEFS_H
#define DEFS_H

#include "csapp.h"
#include <stdbool.h>
#define MAXARGS 128

char* first_args[MAXARGS];
char* rest_args[MAXARGS];

void log_force(char* log);
bool is_argv_pipe(char** argv);
void error_quit(char* msg, char* func_name);

#endif 
