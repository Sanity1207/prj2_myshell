#ifndef RUNNER_HANDLER_C
#define RUNNER_HANDLER_C

#include "runner_handler.h"

void run_command(char** argv, int bg){
    /**
     * Except cd and exit
    */
    int child_status;

    pid_t pid = Fork();

    if(pid==0){ //child process
        if(!strcmp(argv[0],"ls")){
            run_ls(argv);
        }else if(!strcmp(argv[0],"mkdir")){
            run_mkdir(argv);
        }else if(!strcmp(argv[0],"rmdir")){
            run_rmdir(argv);
        }else{
            if(execve(argv[0], argv, environ) < 0) {	//ex) /bin/ls ls -al &
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }

        }

    }else{//parent process
    	/* Parent waits for foreground job to terminate */
        if (!bg){ 
            pid_t wpid = Wait(&child_status);
            if(!WIFEXITED(child_status)){
                printf("Child %d terminated abnormally\n",wpid);
            }
        }else{//when there is backgrount process!
            printf("%d\n", pid);
            //TODO: Do you need to print the command line?

        }

        


    }
    

}

#endif