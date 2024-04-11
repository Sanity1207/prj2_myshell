/* $begin shellmain */
#include "csapp.h"
#include "defs.h"
#include "./myfiles/runner_handler.h"
#include<errno.h>

/* Function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv); 

int main() 
{
    char cmdline[MAXLINE]; /* Command line */

    /**
     * Initialize custom argument arrays
    */
   for (int i = 0; i < MAXARGS; i++) {
    first_args[i] = NULL;
    rest_args[i] = NULL;
    }

    while (1) {
	/* Read */
	printf("CSE4100-SP-P2> ");                   
	fgets(cmdline, MAXLINE, stdin); 
	if (feof(stdin))
	    exit(0);

	/* Evaluate */
	eval(cmdline);
    } 
}
/* $end shellmain */
  
/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline) 
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    pid_t pid;           /* Process id */
    /**
     * ALEX CREATED VARIABLES
    */
    int child_status;
    int pipe_fd[2];
    bool read_from_pipe = false;
    bool write_to_pipe = false;
    bool is_part_of_pipe = false;

    /**
     * END ALEX CREATED VARIABLES
    */
    strcpy(buf, cmdline);
    bg = parseline(buf, argv); 
    if (argv[0] == NULL)  
	return;   /* Ignore empty lines */


    if (!builtin_command(argv)) { //quit -> exit(0), & -> ignore, other -> run
        if(pid==0){ //child process
            if(is_argv_pipe(argv)){
                
            }
            if(!strcmp(argv[0],"ls")){
                Execve("/bin/ls", argv,NULL);
            }else if(!strcmp(argv[0],"mkdir")){
                run_mkdir(argv);
            }else if(!strcmp(argv[0],"rmdir")){
                run_rmdir(argv);
            }else if(!strcmp(argv[0],"cat")){
                run_cat(argv);
            }else if(!strcmp(argv[0],"touch")){
                run_touch(argv);
            }
            else{
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
    


    return;
}

/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv) 
{
    if (!strcmp(argv[0], "exit")){/* quit command */
        exit(0);  
    }else if(!strcmp(argv[0],"cd")){
        run_cd(argv);
        return 1;
    }
    if (!strcmp(argv[0], "&"))    /* Ignore singleton & */
	return 1;
    return 0;                     /* Not a builtin command */
}
/* $end eval */

/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
int parseline(char *buf, char **argv) 
{
    char *delim;         /* Points to first space delimiter */
    int argc;            /* Number of args */
    int bg;              /* Background job? */

    buf[strlen(buf)-1] = ' ';  /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
	buf++;

    /* Build the argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))) {
	argv[argc++] = buf;
	*delim = '\0';
	buf = delim + 1;
	while (*buf && (*buf == ' ')) /* Ignore spaces */
            buf++;
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* Ignore blank line */
	return 1;

    /* Should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0)
	argv[--argc] = NULL;

    return bg;
}
/* $end parseline */


