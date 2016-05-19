# Compiler
CC  = gcc

# Linker
LD  = gcc

# Compiler flags
CF  = -std=gnu99 -Wall -ggdb3 -O0

DEFS = -DSORT

# Linker flags
LF  =

# Libraries
LB  = -lrt -lpthread


# all target for building client and server
all: main

# link results in binary
main: main.o util.o
	$(LD) -o $@ main.o util.o $(LB)

# compile results in object file
main.o: main.c
	$(CC) $(CF) -c $(DEFS) $<

util.o: util.c
	$(CC) $(CF) -c $<

# clean up
clean:
	rm *.o main
