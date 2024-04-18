#ifndef PHASE3_C
#define PHASE3_C

#include "phase3.h"

void add_job(pid_t pid, const char *command) {
    job_t *new_job = Malloc(sizeof(job_t));
    log_force("inside add_job\n");

    new_job->pid = pid;
    new_job->job_id = num_of_jobs+1;

    strcpy(new_job->command, command);


    if(job_list_front == NULL){//list is empty
        new_job->next = NULL;
        new_job->prev = NULL;
        job_list_front = new_job;
    }else{
        new_job->next = job_list_front;
        new_job->prev = NULL;
        job_list_front->prev = new_job;
        job_list_front = new_job;
    }
    num_of_jobs++;

}

void list_jobs() {
    job_t *current_job = job_list_front;
    if(current_job == NULL){
        printf("there is no job to print\n");
    }
    while (current_job != NULL) {
        printf("[%d] %d %s\n", current_job->job_id, current_job->pid, current_job->command);
        current_job = current_job->next;
    }
}

void print_job_list(char* location){
    if(job_list_front == NULL){
        printf("in [%s] : job_list is null\n",location);
    }else{
        printf("in [%s] : job_list is not null\n",location);
        list_jobs();
    }
}



void block_signal(int signum){
    sigset_t mask;
    sigset_t oldmask;

    Sigemptyset(&mask);
    Sigaddset(&mask,signum);
    Sigprocmask(SIG_BLOCK,&mask,&oldmask);

}

void unblock_signal(int signum){
    sigset_t mask;
    sigset_t oldmask;

    Sigemptyset(&mask);
    Sigaddset(&mask,signum);
    Sigprocmask(SIG_UNBLOCK,&mask,&oldmask);
    
}

/**
 * ====================================================================================
 * Signal handlers
 * ====================================================================================
*/


//1. reap the background process
//2. erase the background process from the job list
void sigchld_handler_for_bg(int signum){

    log_to_terminal("signal %d received\n",signum);

    // log_to_terminal("inside signal handler for bg\n");
    int saved_errno = errno;
    int status;

    pid_t pid_bg;
    while((pid_bg = waitpid(-1,&status,WNOHANG)) > 0){ //reaped something
        delete_from_job_list(pid_bg);
        log_to_terminal("inside signal handler reaped process %d\n",pid_bg);
    }

    if (pid_bg == 0) {
        log_to_terminal("no more children to reap - sighandler\n");
    } else if (pid_bg < 0 && errno != ECHILD) {
        perror("waitpid in handler");
    }


    errno = saved_errno;
}

//TODO: problem :what about piped commands? there is only one entry in the list right?
//so, cannot error check.
void delete_from_job_list(pid_t pid){
    job_t* current_job = job_list_front;
    job_t* prev_job = NULL;
    job_t* next_job = NULL;

    log_to_terminal("inside job_list \n");
    log_to_terminal("delete [%d] from job list ",pid);
    if(job_list_front == NULL){ //list is empty
        return;
    }

    while(current_job != NULL){
        if(current_job->pid == pid){
            prev_job = current_job->prev;
            next_job = current_job->next;
            if(prev_job == NULL){//first element in the list
                if(next_job == NULL){//also last element in the list
                    job_list_front = NULL;
                }else{ 
                    job_list_front = next_job;
                    next_job->prev = NULL;
                }
                
            }else if (next_job == NULL){ //last element
                    prev_job->next = NULL;
            }else{//middle element
                prev_job->next = next_job;
                next_job->prev = prev_job;
            }
            free(current_job);
            current_job = NULL;

            num_of_jobs--;
            break;
        }
        
        current_job = current_job->next;
    }
}

#endif