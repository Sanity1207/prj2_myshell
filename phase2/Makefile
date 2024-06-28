CC = gcc
CFLAGS = -Og
LDLIBS = -lpthread

# Object files
OBJS = shellex.o csapp.o defs.o runner_handler.o phase3.o

# Header files
HEADERS = csapp.h defs.h myfiles/runner_handler.h myfiles/phase3.h

# Name of the executable
EXE = myshell

# Default target
all: $(EXE)

# Link the object files into the executable
$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# Compile the source files into object files
shellex.o: shellex.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

csapp.o: csapp.c csapp.h
	$(CC) $(CFLAGS) -c -o $@ $<

defs.o: defs.c defs.h
	$(CC) $(CFLAGS) -c -o $@ $<

runner_handler.o: myfiles/runner_handler.c myfiles/runner_handler.h
	$(CC) $(CFLAGS) -c -o $@ $<

phase3.o: myfiles/phase3.c myfiles/phase3.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean target
clean:
	rm -f $(OBJS) $(EXE)

.PHONY: all clean
