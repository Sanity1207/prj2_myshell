#ifndef PHASE3_H
#define PHASE3_H
#include "../defs.h"
#include "../csapp.h"

void add_job(pid_t pid, const char *command);
void list_jobs();
void print_job_list(char* location);

#endif