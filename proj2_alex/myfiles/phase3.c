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

void print_job_list(char* location){
    if(job_list == NULL){
        printf("in [%s] : job_list is null\n",location);
    }else{
        printf("in [%s] : job_list is not null\n",location);
        list_jobs();
    }
}


//make sure you are passing the correct number through the pipe
//TODO: avoid race conditions.
void sighandler_delete_from_job_list(int signum){
    pid_t pid_to_kill;
    read(pid_pipe[0],&pid_to_kill,sizeof(pid_t));

    job_t* current_job = job_list;
    while(current_job != NULL){
        if(current_job->pid == pid_to_kill){

        }
        current_job = current_job->next;
    }
}

#endif