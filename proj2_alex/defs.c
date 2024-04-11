#ifndef DEFS_C
#define DEFS_C
#include "defs.h"

void log_force(char* log){
    printf("%s\n",log);
    fflush(stdout);
}

/**
 * return: true if "|" in args, false if no pipeline
 * fills up first_args and rest_args
 * first_args: until before |
 * rest_args : rest 
*/
bool is_argv_pipe(char* argv){
    int i;
    int j;
    bool pipeflag = false;
    
    for(i=0;argv[i] != NULL;i++){
        if(!strcmp(argv[i],"|")){ //there is a pipeline
            pipeflag = true;
            break;
        }
    }
    if(pipeflag){
        for(j=0;j<i;j++){
            first_args[j] = argv[j];
        }
        for(j=i+1;argv[j]!=NULL;j++){
            rest_args[j] = argv[j];
        }
    }else{

    }
}
void error_quit(char* msg, char* func_name){
    fprintf(stderr,"Error in %s : %s\n",__func__,msg);
    exit(1);
}



#endif 
