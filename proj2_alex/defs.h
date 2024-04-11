#ifndef DEFS_H
#define DEFS_H

#include "csapp.h"
#include <stdbool.h>
#define MAXARGS 128

char* first_args[MAXARGS];
char* rest_args[MAXARGS];

typedef struct {
    char** argv; 
    bool isPartOfPipeline;
    bool readFromPipe; 
    bool writeToPipe; 
    int fdRead; 
    int fdWrite; 
} command_t;

void log_force(char* log);
bool is_argv_pipe(char** argv);
void error_quit(char* msg, const char* func_name);

#endif 
