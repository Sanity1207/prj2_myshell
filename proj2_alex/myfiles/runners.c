#ifndef RUNNERS_C
#define RUNNERS_C
#include "runners.h"

/**
 * builtin command
*/
void run_cd(char** argv){
    if(argv[1] == NULL){
        if(chdir(getenv("HOME"))){
            fprintf(stderr,"error executing cd");
            exit(1);
        }
    }else{//run chdir 
        if(chdir(argv[1])){
            switch (errno)
            {
            case EACCES:
                printf("cd: %s: Permission denied",argv[1]);
                break;
            case ENOENT:
                printf("bash: cd: %s: No such file or directory\n",argv[1]);
                break;
            case ENOTDIR:
                printf("bash: cd: %s: Not a directory\n",argv[1]);
                break;
            default:
                printf("some other error executing cd\n");
            }
        }
    }
}

/**
 * Not bulitin command
 * already forked. use execve
*/

void run_ls(char** argv){
    Execve("/bin/ls", argv,environ);
    
}

void run_mkdir(char** argv){
    Execve("/bin/mkdir",argv,environ);
}

void run_rmdir(char** argv){
    Execve("/bin/rmdir",argv,environ);
}

void run_touch(char** argv){
    Execve("/usr/bin/touch",argv,environ);
}

void run_cat(char** argv){
    Execve("/bin/cat",argv,environ);
}

void run_grep(char** argv){
    Execve("/bin/grep",argv,environ);
}


#endif