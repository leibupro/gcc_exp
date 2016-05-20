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
HDR  = util.h

SRC  = main.c
SRC += util.c

OBJ  = main.o
OBJ += util.o

ASMDBG  = main_dbg.s
ASMDBG += util_dbg.s

ASMOPT  = main_opt.s
ASMOPT += util_opt.s

BINDBG  = bindbg

BINOPT  = binopt

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
	rename "s/\.s$//_dbg\.s$//" *.s


$(ASMOPT): $(HDR) $(SRC)
	$(CC) $(CFOPT) $(CFARCH) $(CFASM) -S $(SRC)
	rename "s/\.s$//_opt\.s/" *.s
	rename "s/\_dbg_opt/_dbg/" *.s


# clean up
clean:
	rm $(BINDBG) $(BINOPT) $(ASMDBG) $(ASMOPT)

