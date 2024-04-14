#ifndef RUNNER_HANDLER_C
#define RUNNER_HANDLER_C

#include "runner_handler.h"

void fork_handle_command(command_t* command_ptr){
    /**
     * while programming 
    */
    int num_of_commands = num_of_pipes(command_ptr->argv) + 1;

    int pipe_fds[num_of_commands][2];


    int child_status;
    char* cur_command_args[MAXARGS];
    char* rest_command_args[MAXARGS]; 

    int cur_command_idx=0;
    int j;
    int starting_idx = 0;

    //initialize pipe
    init_pipe(pipe_fds,num_of_commands);

    init_command_args(cur_command_args,rest_command_args,command_ptr->argv);
    print_args_with_str("rest_command_args original :",rest_command_args);



    printf("num _ of _commands = %d\n",num_of_commands);

    for(cur_command_idx=0;cur_command_idx<num_of_commands;cur_command_idx++){
        printf("==================cur_command_idx: %d=====================\n",cur_command_idx);
        pid_t pid = Fork();
        if(pid>0){ //Parent Process
            if (!command_ptr->bg){ 
                pid_t wpid = Wait(&child_status);
                if(!WIFEXITED(child_status)){
                    printf("Child %d terminated abnormally\n",wpid);
                
                }
            }else{
                    printf("There is background process : %d\n", pid);
                    //TODO: Do you need to print the command line?
            }
        }else if (pid==0){//child process    
            log_force("inside child process");        
            if(num_of_commands == MAXARGS){ // TODO: fix
                if(cur_command_idx == 0){ //first command
                    if(dup2(pipe_fds[0][1],STDOUT_FILENO)==-1){
                        perror("dup2 in first command");
                    }
                    close(pipe_fds[0][0]);
                }else if (cur_command_idx == num_of_commands-1){// last command
                    if(dup2(pipe_fds[cur_command_idx-1][0],STDIN_FILENO)){
                        perror("dup2 in last command");
                    }
                    close(pipe_fds[cur_command_idx-1][1]);
                }else{ //middle command
                    if(dup2(pipe_fds[cur_command_idx-1][0], STDIN_FILENO)){
                        perror("dup2 in middle/read");
                    }
                    if(dup2(pipe_fds[cur_command_idx-1][1], STDOUT_FILENO)){
                        perror("dup2 in middle/write");
                    }
                }
            }
            
            set_up_current_command_args(cur_command_args,cur_command_idx);
            print_args_with_str("Current Running Command", cur_command_args);
            fflush(stdout);
            run_command(cur_command_args);
            exit(0);
        }else{
            perror("fork");
        }

    
    



    
    }
}

void run_command(char** argv){

    if(!strcmp(argv[0],"ls")){
            run_ls(argv);
    }else if(!strcmp(argv[0],"mkdir")){
            run_mkdir(argv);
    }else if(!strcmp(argv[0],"rmdir")){
            run_rmdir(argv);
    }else if(!strcmp(argv[0],"cat")){
            run_cat(argv);
    }else if(!strcmp(argv[0],"touch")){
            run_touch(argv);
    }else if (!strcmp(argv[0],"grep")){
            run_grep(argv);
    }
    else{
            if(execve(argv[0], argv, environ) < 0) {	//ex) /bin/ls ls -al &
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
        }
}

void init_command_args(char** cur_command_args,char** rest_command_args,char** argv){
    int i;
    for(i=0;i<MAXARGS;i++){
        cur_command_args[i] = NULL;
        rest_command_args[i] = NULL;
    }
    for(i=0;argv[i]!=NULL;i++){
        rest_command_args[i] = malloc(strlen(argv[i])+1);
        strcpy(rest_command_args[i],argv[i]);
    }
}

//The first command arguments of the rest_command_args goes into cur_command args
//and is removed from rest_command_args
void set_up_command_args(char** cur_command_args,char** rest_command_args,int* starting_idx){
    int i;
    int j=0;
    //free all memory from cur_command_args
    for(i=0;cur_command_args[i]!=NULL;i++){
        free(cur_command_args[i]);
        cur_command_args[i] = NULL; 
    }
    printf("starting idx start= %d\n",*starting_idx);
    
    //set up first command
    for(i=(*starting_idx);rest_command_args[i] != NULL && strcmp(rest_command_args[i], "|") != 0;i++){
        cur_command_args[j] = malloc(strlen(rest_command_args[i]) + 1);
        if (cur_command_args[j] == NULL) {
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }
        strcpy(cur_command_args[j], rest_command_args[i]);
        j++;
    }
    cur_command_args[j] = NULL;
    *starting_idx = i+1; 
    printf("starting idx end= %d\n",*starting_idx);
}

void set_up_current_command_args(char** cur_command_args,int cur_command_idx){
    //그냥 매번 구해야함. 
}


void init_pipe(int pipe_fds[][2],int num_of_commands){
    int i;

    for(i=0;i<num_of_commands;i++){
        if(pipe(pipe_fds[i])<0){
            perror("piping error in init_pipe");
        }
    }
}


#endif