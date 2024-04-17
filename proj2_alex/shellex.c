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
    job_list = NULL;
    num_of_jobs = 0;

    while (1) {
	/* Read */
	printf("CSE4100-SP-P2> ");                   
	fgets(cmdline, MAXLINE, stdin); 
	if (feof(stdin))
	    exit(0);
    /**
     * Alex set up variables
    */

    // print_job_list("main");
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
    int i;

    /**
     * Alex created variables
    */
    command_t *command_struct = malloc(sizeof(command_struct));
    command_struct->command = NULL;
    /**
     * Initialize argv
    */
    for(i=0;i<MAXARGS;i++){
        argv[i] = NULL;
    }


    strcpy(buf, cmdline);
    bg = parseline(buf, argv); 
    if (argv[0] == NULL)  
	return;   /* Ignore empty lines */

    if(bg){//there is background process, need to access commandline.
        command_struct->command = malloc(MAXLINE);
        strcpy(command_struct->command, cmdline);
    }

    if (!builtin_command(argv)) { //quit -> exit(0), & -> ignore, other -> run
        command_struct->argv = argv;
        command_struct->bg = bg; 
        fork_handle_command(command_struct);
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
    int bgIdx;
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
    
    // check for & right after string without space.
    bgIdx = strlen(argv[argc-1])-1;
    if((argv[argc-1])[bgIdx] == '&'){
        (argv[argc-1])[bgIdx] = '\0';
        return 1;
    }


    /* Should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0)
	argv[--argc] = NULL;


    return bg;
}
/* $end parseline */


