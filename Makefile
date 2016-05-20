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

# Compiler
CC  = gcc

# Linker
LD  = gcc

# Compiler flags
CFDBG  = -std=gnu99 -Wall -ggdb3 -O0
CFOPT  = -std=gnu99 -funroll-loops -O3

CFARCH = -march=native

CFASM  = -fverbose-asm -masm=intel

# Linker flags
LF  =

# Libraries
LB  = -lrt -lpthread

# Files
MAIN = main
UTIL = util

HDR  = $(UTIL).h

SRC  = $(MAIN).c
SRC += $(UTIL).c

OBJ  = $(MAIN).o
OBJ += $(UTIL).o

ASMDBG  = $(MAIN)_dbg.s
ASMDBG += $(UTIL)_dbg.s

ASMOPT  = $(MAIN)_opt.s
ASMOPT += $(UTIL)_opt.s

BINDBG  = bin_dbg

BINOPT  = bin_opt

# phony targets
.PHONY: all clean


# all targets
all: $(BINDBG) $(BINOPT) $(ASMDBG) $(ASMOPT)


$(BINDBG): $(HDR) $(SRC)
	$(CC) $(CFDBG) $(CFARCH) -c $(SRC)
	$(LD) -o $@ $(OBJ) $(LB)
	rm $(OBJ)	


$(BINOPT): $(HDR) $(SRC)
	$(CC) $(CFOPT) $(CFARCH) -c $(SRC)
	$(LD) -o $@ $(OBJ) $(LB)
	rm $(OBJ)


$(ASMDBG): $(HDR) $(SRC)
	$(CC) $(CFDBG) $(CFARCH) $(CFASM) -S $(SRC)
	mv $(MAIN).s $(word 1,$(ASMDBG))
	mv $(UTIL).s $(word 2,$(ASMDBG))


$(ASMOPT): $(HDR) $(SRC)
	$(CC) $(CFOPT) $(CFARCH) $(CFASM) -S $(SRC)
	mv $(MAIN).s $(word 1,$(ASMOPT))
	mv $(UTIL).s $(word 2,$(ASMOPT))


# clean up
clean:
	rm $(BINDBG) $(BINOPT) $(ASMDBG) $(ASMOPT)

