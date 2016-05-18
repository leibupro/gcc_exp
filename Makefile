# Compiler
CC  = gcc

# Linker
LD  = gcc

# Compiler flags
CF  = -std=gnu99 -Wall -ggdb3 -O0

DEFS  = -DSPLASH_SINGLE
DEFS += -DSORT

# Linker flags
LF  =

# Libraries
LB  = -lrt


# all target for building client and server
all: main

# link results in binary
main: main.o
	$(LD) -o $@ main.o $(LB)

# compile results in object file
main.o: main.c
	$(CC) $(CF) -c $(DEFS) $<

# clean up
clean:
	rm *.o main
