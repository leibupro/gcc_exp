# Compiler
CC     = gcc

# Compiler flags
CF     = -std=gnu99

CFARCH = -march=native

CFDBG  = -O0
CFOPT  = -funroll-loops -O3

CFASM  = -fverbose-asm -masm=intel

# Directories
SRCDIR = ./src
ASMDIR = ./asm

# Files
SRC    = $(SRCDIR)/unroll_1.c
SRC   += $(SRCDIR)/unroll_2.c

ASMDBG = $(subst $(SRCDIR),$(ASMDIR),$(SRC:.c=_dbg.s))
ASMOPT = $(subst $(SRCDIR),$(ASMDIR),$(SRC:.c=_opt.s))


# vpath variable for pattern rules to look in the
# directories specified in vpath as well when searching
# the dependency files.
VPATH = $(SRCDIR)


# phony targets
.PHONY: all clean


all: $(ASMDIR) $(ASMDBG) $(ASMOPT)


$(ASMDIR):
	mkdir $@


$(ASMDIR)/%_dbg.s: %.c
	$(CC) $(CFARCH) $(CFDBG) $(CFASM) -S $< -o $@


$(ASMDIR)/%_opt.s: %.c
	$(CC) $(CFARCH) $(CFOPT) $(CFASM) -S $< -o $@


clean:
	rm -r $(ASMDIR)

