#ifndef RUNNER_HANDLER_H
#define RUNNER_HANDLER_H

#include "../csapp.h"
#include "runners.h"

void run_command(char** argv, int bg, int* read_fd, int* write_fd);



#endif