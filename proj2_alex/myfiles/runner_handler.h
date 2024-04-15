#ifndef RUNNER_HANDLER_H
#define RUNNER_HANDLER_H

#include "../csapp.h"
#include "runners.h"

void fork_handle_command(command_t* command_ptr);
void run_command(char** argv);
void init_command_args(char** cur_command_args,char** rest_command_args,char** argv);
void init_pipe(int pipe_fds[][2],int num_of_commands);
void set_up_command_args(char** cur_command_args,char** rest_command_args,int* starting_idx);
void set_up_current_command_args(char** cur_command_args,char** argv,int cur_command_idx);

void nullify_str_arr(char** cur_command_args, int size);

void remove_quotes_from_argv(char** argv);




#endif