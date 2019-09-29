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
OBJDIR = ./obj
BINDIR = ./bin
ASMDIR = ./asm


# Compiler
CC  = gcc


# Linker
LD  = gcc


# Compiler flags
CF     = -std=gnu99

CFARCH = -march=native

CFDBG  = -Wall -ggdb3 -O0
CFOPT  = -funroll-loops -O3

CFASM  = -fverbose-asm -masm=intel

INCLUDES = -I$(INCDIR)


# Linker flags
LF  =


# Libraries
LB  = -lrt -lpthread


# Files
HDR  = $(INCDIR)/util.h

SRC  = $(SRCDIR)/main.c
SRC += $(SRCDIR)/util.c

OBJDBG = $(subst $(SRCDIR),$(OBJDIR),$(SRC:.c=_dbg.o))
OBJOPT = $(subst $(SRCDIR),$(OBJDIR),$(SRC:.c=_opt.o))

ASMDBG = $(subst $(SRCDIR),$(ASMDIR),$(SRC:.c=_dbg.s))
ASMOPT = $(subst $(SRCDIR),$(ASMDIR),$(SRC:.c=_opt.s))

BINDBG = $(BINDIR)/bin_dbg
BINOPT = $(BINDIR)/bin_opt


# vpath variable for pattern rules to look into the
# directories specified in vpath as well when 
# searching the dependency files.
VPATH = $(SRCDIR)


# phony targets
.PHONY: all clean


# all files/directories we want in the end ...
all: $(BINDBG) $(BINOPT) $(ASMDBG) $(ASMOPT)


$(OBJDIR):
	mkdir $@


$(ASMDIR):
	mkdir $@


$(BINDIR):
	mkdir $@


$(BINDBG): $(OBJDBG) $(BINDIR)
	$(LD) -o $@ $(OBJDBG) $(LB)


$(BINOPT): $(OBJOPT) $(BINDIR)
	$(LD) -o $@ $(OBJOPT) $(LB)


$(ASMDIR)/%_dbg.s: %.c $(HDR) $(ASMDIR)
	$(CC) $(CF) $(CFDBG) $(CFASM) $(INCLUDES) -S $< -o $@


$(ASMDIR)/%_opt.s: %.c $(HDR) $(ASMDIR)
	$(CC) $(CF) $(CFARCH) $(CFOPT) $(CFASM) $(INCLUDES) -S $< -o $@


$(OBJDIR)/%_dbg.o: %.c $(HDR) $(OBJDIR)
	$(CC) $(CF) $(CFDBG) $(INCLUDES) -c $< -o $@


$(OBJDIR)/%_opt.o: %.c $(HDR) $(OBJDIR)
	$(CC) $(CF) $(CFARCH) $(CFOPT) $(INCLUDES) -c $< -o $@


# clean up
clean:
	-rm -rf $(OBJDIR) $(BINDIR) $(ASMDIR)

