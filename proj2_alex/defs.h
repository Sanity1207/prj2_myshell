#ifndef DEFS_H
#define DEFS_H

#include "csapp.h"
#include <stdbool.h>
#define MAXARGS 128

typedef struct{
    char** argv; //array of argument strings.
    int bg; //whether there is a background process or not
    char* command; //save the command line if necessary  
} command_t;

typedef struct job {
    int job_id;
    pid_t pid;
    char command[MAXLINE];
    struct job *next;
} job_t;

job_t *job_list;
int num_of_jobs;


void log_force(char* log);
bool is_argv_pipe(char** argv);
void error_quit(char* msg, const char* func_name);
void print_args_with_str(char* str, char** args);
int num_of_pipes(char** argv);
void log_to_terminal(const char* format, ...);
void remove_first_two_letters(char* buf);

#endif 
