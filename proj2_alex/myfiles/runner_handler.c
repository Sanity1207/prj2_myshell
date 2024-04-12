#ifndef RUNNER_HANDLER_C
#define RUNNER_HANDLER_C

#include "runner_handler.h"

void fork_handle_command(command_t* command_ptr){
 
    /**
     * while programming 
    */
    int num_of_pipe = num_of_pipes(command_ptr->argv);
   /**
     * Except cd and exit
    */
    int child_status;
    int i;


    

    pid_t pid = Fork();
    


    if(pid>0){ //Parent Process
        if (!command_ptr->bg){ 
            pid_t wpid = Wait(&child_status);
            if(!WIFEXITED(child_status)){
                printf("Child %d terminated abnormally\n",wpid);
            }
        }else{//when there is backgrount process!
            printf("%d\n", pid);
            //TODO: Do you need to print the command line?
        }
    }else{//child process
        print_args_with_str("child after fork",command_ptr->argv);

        if(command_ptr->child_has_prev_input){ //has a previous input from another file
            dup2(command_ptr->pipe_fd[0],STDIN_FILENO);
        }else{
            close(command_ptr->pipe_fd[0]); //close the reading end of the pipe
        }


        if(is_argv_pipe(command_ptr->argv)){ // gotta feed another child.
            log_force("in pipe_command");
            dup2(command_ptr->pipe_fd[1],STDOUT_FILENO);
            command_ptr->argv = first_args;
            run_command(command_ptr->argv);
            command_ptr->argv = rest_args;
            fork_handle_command(command_ptr);
        }else{ //no pipe after me
            // log_force("in run_command");
            run_command(command_ptr->argv);
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


#endif