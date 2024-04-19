CC = gcc
CFLAGS = -Og
LDLIBS = -lpthread

PROGS = shellex

all: $(PROGS)

shellex: shellex.c csapp.c defs.c myfiles/runner_handler.c myfiles/phase3.c

clean:
	rm -rf *~ $(PROGS)

