CC = gcc
CFLAGS = -Og
LDLIBS = -lpthread

PROGS = shellex

all: $(PROGS)

shellex: shellex.c csapp.c defs.c myfiles/runners.c myfiles/runner_handler.c

clean:
	rm -rf *~ $(PROGS)

