#ifndef PHASE3_H
#define PHASE3_H
#include "../defs.h"
#include "../csapp.h"

void add_job(pid_t pid, const char *command);
void list_jobs();
void print_job_list(char* location);
void block_signal(int signum);
void unblock_signal(int signum);
void delete_from_job_list(pid_t pid);


void sigchld_handler_for_bg();

#endif