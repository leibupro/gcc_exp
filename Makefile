#  ############################################################
#
#
#   File:          Makefile
#
#
#   Purpose:       Play around with the GCC
#
#                  This makefile generates
#                  two binaries:
#
#                  - One without optimization
#                    and debugging symbols
#
#                  - One with massive
#                    optimizations which
#                    the GCC provides us.
#
#                  Further on the makefile
#                  generates readable 
#                  assembler code in a very
#                  verbose manner for both
#                  optimized and debugging
#                  compiled sources.
#    
#
#   Remarks:       -
# 
#
#   Author:        P. Leibundgut <leiu@zhaw.ch>
#                                <pl@vqe.ch>
#
#
#   Date:          05/2016
#
#
#  ############################################################

# Directories
INCDIR = ./include
SRCDIR = ./src
BINDIR = ./bin
ASMDIR = ./asm


# Compiler
CC  = gcc


# Linker
LD  = gcc


# Compiler flags
CFDBG  = -std=gnu99 -Wall -ggdb3 -O0
CFOPT  = -std=gnu99 -funroll-loops -O3

CFARCH = -march=native

CFASM  = -fverbose-asm -masm=intel

INCLUDES = -I$(INCDIR)


# Linker flags
LF  =


# Libraries
LB  = -lrt -lpthread


# Files
MAIN = main
UTIL = util

HDR  = $(INCDIR)/$(UTIL).h

SRC  = $(SRCDIR)/$(MAIN).c
SRC += $(SRCDIR)/$(UTIL).c

OBJ  = $(MAIN).o
OBJ += $(UTIL).o

ASMDBG  = $(ASMDIR)/$(MAIN)_dbg.s
ASMDBG += $(ASMDIR)/$(UTIL)_dbg.s

ASMOPT  = $(ASMDIR)/$(MAIN)_opt.s
ASMOPT += $(ASMDIR)/$(UTIL)_opt.s

BINDBG  = $(BINDIR)/bin_dbg

BINOPT  = $(BINDIR)/bin_opt


# phony targets
.PHONY: all clean


# all targets
all: $(BINDBG) $(BINOPT) $(ASMDBG) $(ASMOPT)


$(BINDBG): $(HDR) $(SRC)
	mkdir -p $(BINDIR)
	$(CC) $(CFDBG) $(CFARCH) -c $(INCLUDES) $(SRC)
	$(LD) -o $@ $(OBJ) $(LB)
	rm $(OBJ)	


$(BINOPT): $(HDR) $(SRC)
	mkdir -p $(BINDIR)
	$(CC) $(CFOPT) $(CFARCH) -c $(INCLUDES) $(SRC)
	$(LD) -o $@ $(OBJ) $(LB)
	rm $(OBJ)


$(ASMDBG): $(HDR) $(SRC)
	mkdir -p $(ASMDIR)
	$(CC) $(CFDBG) $(CFARCH) $(CFASM) -S $(INCLUDES) $(SRC)
	mv $(MAIN).s $(word 1,$(ASMDBG))
	mv $(UTIL).s $(word 2,$(ASMDBG))


$(ASMOPT): $(HDR) $(SRC)
	mkdir -p $(ASMDIR)
	$(CC) $(CFOPT) $(CFARCH) $(CFASM) -S $(INCLUDES) $(SRC)
	mv $(MAIN).s $(word 1,$(ASMOPT))
	mv $(UTIL).s $(word 2,$(ASMOPT))


# clean up
clean:
	rm $(BINDBG) $(BINOPT) $(ASMDBG) $(ASMOPT)
	rmdir $(BINDIR) $(ASMDIR)

