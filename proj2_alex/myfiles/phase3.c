#ifndef PHASE3_C
#define PHASE3_C

#include "phase3.h"

void add_job(pid_t pid, const char *command) {
    job_t *new_job = malloc(sizeof(job_t));
    new_job->pid = pid;
    strcpy(new_job->command, command);
    new_job->next = job_list;
    job_list = new_job;
    num_of_jobs++;

    if(job_list == NULL){
        printf("job list is NULL\n");
    }else{
        printf("successfully added job");
    }
}

void list_jobs() {
    job_t *current_job = job_list;
    if(current_job == NULL){
        printf("there is no job to print\n");
    }
    while (current_job != NULL) {
        printf("[%d] %d %s\n", current_job->job_id, current_job->pid, current_job->command);
        current_job = current_job->next;
    }
}

#endif