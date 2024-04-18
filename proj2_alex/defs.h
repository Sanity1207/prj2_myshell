#ifndef DEFS_H
#define DEFS_H
#include "csapp.h"
#include <stdbool.h>
#include <stddef.h>
#define MAXARGS 128

#ifndef SIG_BLOCK
#define SIG_BLOCK          0        /* for blocking signals */
#endif
#ifndef SIG_UNBLOCK
#define SIG_UNBLOCK        1        /* for unblocking signals */
#endif
#ifndef SIG_SETMASK
#define SIG_SETMASK        2        /* for setting the signal mask */
#endif



typedef struct{
    char** argv; //array of argument strings.
    int bg; //whether there is a background process or not
    char* command; //save the command line if necessary  
    bool already_added;
} command_t;

typedef struct job {
    int job_id;
    pid_t pid;
    char command[MAXLINE];
    struct job *next;
    struct job *prev;
} job_t;

//phase 3 variables
job_t *job_list_front;
int num_of_jobs;
int pid_pipe[2];


void log_force(char* log);
bool is_argv_pipe(char** argv);
void error_quit(char* msg, const char* func_name);
void print_args_with_str(char* str, char** args);
int num_of_pipes(char** argv);
void log_to_terminal(const char* format, ...);
void remove_first_two_letters(char* buf);

#endif 
