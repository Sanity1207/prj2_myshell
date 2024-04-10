#ifndef RUNNER_HANDLER_C
#define RUNNER_HANDLER_C

#include "./csapp.h"

void run_command(char** argv){
    /**
     * Except cd and exit
    */
    if(!strcmp(argv[0],"exit")){
        run_exit(argv);
    }else if(!strcmp(argv[0],"cd")){
        run_cd(argv);
    }

    pid_t pid = fork();

    if(pid==0){
        //child process
        if(!strcmp(argv[0],"ls")){
            run_ls(argv);
        }
    }
    

}

#endif