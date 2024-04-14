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
    int cur_command_idx=0;
    int j;
    int k;
    int wiping_idx;
    int starting_idx = 0;

    //initialize pipe
    init_pipe(pipe_fds,num_of_commands);

    //initialize cur_command_args
    nullify_str_arr(cur_command_args);


    // printf("num _ of _commands = %d\n",num_of_commands);

    for(cur_command_idx=0;cur_command_idx<num_of_commands;cur_command_idx++){
        // printf("==================cur_command_idx: %d=====================\n",cur_command_idx);
        pid_t pid = Fork();
        if(pid>0){ //Parent Process
            if (!command_ptr->bg){ 
                pid_t wpid = Wait(&child_status);
                if(cur_command_idx != 0){//not the first command
                    close(pipe_fds[cur_command_idx-1][0]); //close read end of the pipe that Iused
                }

                if(cur_command_idx != num_of_commands-1){//not the last command
                    close(pipe_fds[cur_command_idx][1]); //close write end of pipe I used
                }

                if(cur_command_idx == num_of_commands-1){//last command, free all memory
                    for(wiping_idx = 0;wiping_idx<MAXARGS;wiping_idx++){
                        if(cur_command_args[wiping_idx] != NULL){
                            free(cur_command_args[wiping_idx]);
                            cur_command_args[wiping_idx] = NULL;
                        }
                    }
                }

                if(!WIFEXITED(child_status)){
                    printf("Child %d terminated abnormally\n",wpid);
                }
            }else{
                    printf("There is background process : %d\n", pid);
                    //TODO: Do you need to print the command line?
            }
        }else if (pid == 0) {  // Child process
            // Setup redirection
            if (cur_command_idx != 0) {  // Not the first command
                if (dup2(pipe_fds[cur_command_idx - 1][0], STDIN_FILENO) == -1) {
                    perror("dup2 for stdin failed");
                    exit(EXIT_FAILURE);
                }
            }
            if (cur_command_idx != num_of_commands - 1) {  // Not the last command
                if (dup2(pipe_fds[cur_command_idx][1], STDOUT_FILENO) == -1) {
                    perror("dup2 for stdout failed");
                    exit(EXIT_FAILURE);
                }
            }

            // Execute the command
            set_up_current_command_args(cur_command_args,command_ptr->argv,cur_command_idx);
            run_command(cur_command_args);
            exit(0);

        } else if (pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }else{
            perror("fork");
        }
    }    
}

void run_command(char** argv){
    //TODO: 파일의 경로를 통해 execvp 를 자동으로 실행하게 하기
    //TODO: environmental variable 이 대체 무엇인지 알아내기

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


//only call when the command is "piped"
//current command idx 에 해당하는 argument fetch.

void set_up_current_command_args(char** cur_command_args,char** argv,int cur_command_idx){
    //그냥 매번 구해야함.
    int current_argument_idx = cur_command_idx; 
    int i;
    int idx = 0; //현재 몇 번째의 커맨드를 가르키고 있는지
    int j = 0; //index for cur_command_args array
    
    // printf("cur_command_idx = %d\n",current_argument_idx);


    //free the command_args array
    for(i=0;cur_command_args[i]!=NULL;i++){
        free(cur_command_args[i]);
        cur_command_args[i] = NULL;
    }
    
    for(i=0;argv[i]!=NULL;i++){
        if(!strcmp(argv[i],"|")){
            idx++;
            if(current_argument_idx > idx){
                break;
            }
            continue;
        }
        
        if(current_argument_idx == idx){//start recording
            cur_command_args[j] = malloc(strlen(argv[i])+1);
            if (cur_command_args[j] == NULL) {
                perror("Failed to allocate memory");
                exit(EXIT_FAILURE);
            }
            strcpy(cur_command_args[j],argv[i]);
            // printf("recorded argument : %s\n",cur_command_args[j]);
            j++;
        }
        


    }
    cur_command_args[j] = NULL;
}


void init_pipe(int pipe_fds[][2],int num_of_commands){
    int i;

    for(i=0;i<num_of_commands;i++){
        if(pipe(pipe_fds[i])<0){
            perror("piping error in init_pipe");
        }
    }
}

//do not free. just nullify the whole array size of maxargs
void nullify_str_arr(char** cur_command_args){
    int i;
    for(i=0;i<MAXARGS;i++){
        cur_command_args[i] = NULL;
    }
}

//1. free and nullify dest
//2. copy src into dest
void copy_str_args(char** dest,char** src){
    int i;

    //free and nullify dest
    for(i=0;dest[i]!=NULL;i++){
        free(dest[i]);
        dest[i] = NULL; 
    }

    for(i=0;src[i]!=NULL;i++){
        dest[i] = malloc(strlen(src[i])+1);
        strcpy(dest[i],src[i]);
    }
}


/***
 * Deprecated
*/

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

#endif