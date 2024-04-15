#ifndef DEFS_C
#define DEFS_C
#include "defs.h"

void log_force(char* log) {
    int fd = open("/dev/tty", O_WRONLY);
    if (fd == -1) {
        perror("open /dev/tty");
        return;
    }
    
    dprintf(fd, "%s\n", log);
    close(fd);
}

/**
 * return: true if "|" in args, false if no pipeline
 * fills up first_args and rest_args
 * first_args: until before |
 * rest_args : rest 
*/
void error_quit(char* msg, const char* func_name){
    fprintf(stderr,"Error in %s : %s\n",__func__,msg);
    exit(1);
}

void print_args_with_str(char* str, char** args){
    int i;
    printf("'%s' args : ",str);
    for(i=0;args[i] != NULL;i++){
        printf("'%s' ",args[i]);
    }
    if(i==0){
        printf("args are empty!\n");
    }else{
        printf("\n");
    }
}

int num_of_pipes(char** argv){
    int i;
    int cnt = 0;
    for(int i = 0;argv[i]!= NULL;i++){
        if(!strcmp(argv[i],"|")){
            cnt++;
        }
    }
    return cnt;
}

void log_to_terminal(const char* format, ...) {
    int fd = open("/dev/tty", O_WRONLY);
    if (fd == -1) {
        perror("open /dev/tty failed");
        return;
    }

    char buffer[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    write(fd, buffer, strlen(buffer));
    close(fd);
}
#endif 
