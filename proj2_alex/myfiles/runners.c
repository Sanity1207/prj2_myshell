#ifndef RUNNERS_C
#define RUNNERS_C

#include "../csapp.h"
#include "runners.h"

void run_cd(char** argv){
    if(argv[1] == NULL){
        chdir("~");
    }else{//run chdir 
        chdir(argv[1]);
    //     {
    //     case EACCES:
    //         printf("cd: %s: Permission denied",argv[1]);
    //         break;
    //     case ENOENT:
    //         printf("bash: cd: %s: No such file or directory",argv[1]);
    //     case ENOTDIR
    //         printf("bash: cd: %s: No such file or directory",argv[1]);

    //     default:
    //         break;
    //     }
    // }
    }
}
/**
 * already forked. use execve
*/
void run_ls(char** argv){
    Execve("/bin/ls", argv,NULL);
}

void run_mkdir(char** argv){
    Execve("/bin/mkdir",argv,NULL);
}

void run_rmdir(char** argv){
    Execve("/bin/rmdir",argv,NULL);
}



#endif