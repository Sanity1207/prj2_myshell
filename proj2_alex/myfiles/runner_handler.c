#ifndef RUNNER_HANDLER_C
#define RUNNER_HANDLER_C

#include "runner_handler.h"

void fork_handle_command(command_t* command_ptr){
    /**
     * while programming 
    */
    int num_of_commands = num_of_pipes(command_ptr->argv) + 1;

    //phase 2 piping 
    int pipe_fds[num_of_commands-1][2];

    //phase 3 piping
    int pid_pipe[2];


    int child_status;
    char* cur_command_args[MAXARGS];
    int cur_command_idx=0;
    int j;
    int k;
    int wiping_idx;
    int starting_idx = 0;
    int logging_idx = 0;

    //initialize pipe
    init_pipe(pipe_fds,num_of_commands);
    if(pipe(pid_pipe)<0){
        perror("piping error for pid_pipe");
        exit(1);
    }

    //initialize cur_command_args
    nullify_str_arr(cur_command_args,MAXARGS);

    for(cur_command_idx=0;cur_command_idx<num_of_commands;cur_command_idx++){
        // printf("==================cur_command_idx: %d=====================\n",cur_command_idx);
        pid_t pid = Fork();
        if(pid>0){ //Parent Process
            // print_job_list("right after fork (parent)");
            if (!command_ptr->bg){ //no background process
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
                    printf("exit status : %d",child_status);
                }
            }else{ //background process
                add_job(pid,command_ptr->command);
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
            remove_quotes_from_argv(cur_command_args);


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
    char command_name[10];
    char bin_path[30];
    char usr_bin_path[30];

    int i;
    




    //1. fetch the first argument
    strcpy(command_name,argv[0]);

    //2. check if the command wants to run some exec file
    if(command_name[0] == '.' && command_name[1] == '/'){
        execv(command_name,argv);
    }

    //3. check if the command needs to be custom handled.

    if(!strcmp(command_name,"jobs")){
        printf("listing jobs\n");
        list_jobs();
    }






    //4. make a path with the argument
    strcpy(bin_path,"/bin/");
    strcpy(usr_bin_path,"/usr/bin/");

    strcat(bin_path,command_name);
    strcat(usr_bin_path,command_name);

    execvp(bin_path,argv);
    execvp(usr_bin_path,argv);
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
    for(i=0;i<MAXARGS;i++){
        if(cur_command_args[i]!=NULL){
            free(cur_command_args[i]);
            cur_command_args[i] = NULL;
        }
    }
    
    for(i=0;argv[i]!=NULL;i++){
        if(!strcmp(argv[i],"|")){
            idx++;
            if(current_argument_idx < idx){
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
            j++;
        }
    }
    cur_command_args[j] = NULL;
}


void init_pipe(int pipe_fds[][2],int num_of_commands){
    int i;

    for(i=0;i<num_of_commands-1;i++){
        if(pipe(pipe_fds[i])<0){
            perror("piping error in init_pipe");
        }
    }
}

//do not free. just nullify the whole array size: param size
void nullify_str_arr(char** cur_command_args,int size){
    int i;
    for(i=0;i<size;i++){
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

void remove_quotes_from_argv(char** argv){
    int i = 0;
    int j = 0;
    int c;
    char arg_new[100];
    for(i=0;argv[i]!=NULL;i++){
        c=0;
        for(j=0;j < strlen(argv[i]);j++){
            if(argv[i][j] != '\"'){
                arg_new[c] = argv[i][j];
                c++;
            }
        }
        arg_new[c] = '\0';
        strcpy(argv[i],arg_new);
        strcpy(arg_new,"\0");
    }
}



#endif