#ifndef RUNNER_HANDLER_H
#define RUNNER_HANDLER_H

#include "../csapp.h"
#include "runners.h"

void fork_handle_command(command_t* command_ptr);
void run_command(char** argv);



#endif