#ifndef PHASE3_C
#define PHASE3_C

#include "phase3.h"

void add_job(pid_t pid, const char *command, int bg) {
    job_t *new_job = Malloc(sizeof(job_t));
    // log_force("inside add_job\n");

    job_t *current_job;

    new_job->pid = pid;
    new_job->job_id = id_for_next_job;
    new_job->recency = CURRENT;
    if(bg){
        new_job->is_bg = true;
    }else{
        new_job->is_bg = false;
    }

    strcpy(new_job->process_state,"RUNNING");
    

    strcpy(new_job->command, command);



    if(job_list_front == NULL){//list is empty
        new_job->next = NULL;
        new_job->prev = NULL;
        job_list_front = new_job;
    }else{
        current_job = job_list_front;
        while(current_job->next != NULL){
            if(current_job->recency == CURRENT){
                current_job->recency = PREVIOUS;
            }else if (current_job->recency == PREVIOUS){
                current_job->recency = NORMAL;
            }
            current_job = current_job->next;
        }

        //the next node of currentjob is null.
        current_job->next = new_job;
        new_job->prev = current_job;

    }
    id_for_next_job++;
}

void list_jobs(char* option) {
    job_t *current_job = job_list_front;
    char recency_char;




    if(option == NULL){
        fflush(stdout);
        while (current_job != NULL) {
            if(current_job->recency == CURRENT){
                recency_char = '+';
            }else if (current_job->recency == PREVIOUS){
                recency_char = '-';
            }else{
                recency_char = ' ';
            }
            printf("[%d]%c %s\t\t%s",current_job->job_id,recency_char,current_job->process_state,current_job->command);
            current_job = current_job->next;
        }
    }else if(!strcmp(option,"-l")){
        while (current_job != NULL) {
            if(current_job->recency == CURRENT){
                recency_char = '+';
            }else if (current_job->recency == PREVIOUS){
                recency_char = '-';
            }else{
                recency_char = ' ';
            }
            printf("[%d]%c %d %s\t\t%s",current_job->job_id,recency_char,current_job->pid,current_job->process_state,current_job->command);
            current_job = current_job->next;
        }
    }

}

void print_job_list(char* location){
    if(job_list_front == NULL){
        printf("in [%s] : job_list is null\n",location);
    }else{
        printf("in [%s] : job_list is not null\n",location);
        list_jobs(NULL);
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

pid_t get_job_pid_with_job_id(int job_id){
    job_t* current_job;

    current_job = job_list_front;
    while(current_job != NULL){
        log_to_terminal("current_job_id = %d, search job id = %d\n",current_job->job_id,job_id);
        if(current_job->job_id == job_id){
            log_to_terminal("[fg] chosen_job_id = %d\n",current_job->pid);
            return current_job->pid;
        }
        current_job = current_job->next;
    }
    return -1;
    
    // error_quit("could not find the job with job_id [%d]\n",__func__);
}
// void install_parent_handler_for_sigstop(){
//     struct sigaction sa_sigchld;
//     struct sigaction oldaction_sigchld;
//     sa_sigchld.sa_handler = sigstop_handler_for_parent;
//     Sigemptyset(&sa_sigchld.sa_mask);
//     Sigaddset(&sa_sigchld.sa_mask,SIGSTOP);
//     sa_sigchld.sa_flags = SA_RESTART;

//     if(sigaction(SIGSTOP,&sa_sigchld,&oldaction_sigchld) < 0){
//         unix_error("Signal Error_SIGSTOP");
//     }
// }
// void install_parent_handler_for_sigint(){
//     struct sigaction sa_sigchld;
//     struct sigaction oldaction_sigchld;
//     sa_sigchld.sa_handler = sigint_handler_for_parent;
//     Sigemptyset(&sa_sigchld.sa_mask);
//     Sigaddset(&sa_sigchld.sa_mask,SIGINT);
//     sa_sigchld.sa_flags = SA_RESTART;

//     if(sigaction(SIGSTOP,&sa_sigchld,&oldaction_sigchld) < 0){
//         unix_error("Signal Error_SIGINT");
//     }
// }




/**
 * ====================================================================================
 * Signal handlers
 * ====================================================================================
*/


//1. reap the background process
//2. erase the background process from the job list
void sigchld_handler_for_bg(int signum){

    // log_to_terminal("signal %d received\n",signum);

    // log_to_terminal("inside signal handler for bg\n");
    int saved_errno = errno;
    int status;

    pid_t pid_bg;
    while((pid_bg = waitpid(-1,&status,WNOHANG)) > 0){ // 0이면 탈출한다.
        delete_from_job_list(pid_bg);
        // log_to_terminal("inside signal handler reaped process %d\n",pid_bg);
    }

    if (pid_bg == 0) {
        // log_to_terminal("no more children to reap - sighandler\n");
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
    
    if(job_list_front == NULL){ //list is empty
        return;
    }

    while(current_job != NULL){
        if(current_job->pid == pid){
            prev_job = current_job->prev;
            next_job = current_job->next;
            if(next_job == NULL){//must decrement id for next
                id_for_next_job--;
            }

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
            // log_to_terminal("deleted [%d] from job list \n",pid);
            break;
            
        }
        
        current_job = current_job->next;
    }
}

#endif