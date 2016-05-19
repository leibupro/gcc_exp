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


# phony targets
.PHONY: all clean asmdbg asmopt


# all targets
all: bindbg binopt asmdbg asmopt


bindbg: $(HDR) $(SRC)
	$(CC) $(CFDBG) $(CFARCH) -c $(SRC)
	$(LD) -o $@ $(OBJ) $(LB)
	rm $(OBJ)	


binopt: $(HDR) $(SRC)
	$(CC) $(CFOPT) $(CFARCH) -c $(SRC)
	$(LD) -o $@ $(OBJ) $(LB)
	rm $(OBJ)


asmdbg: $(HDR) $(SRC)
	$(CC) $(CFDBG) $(CFARCH) $(CFASM) -S $(SRC)
	rename .s _dbg.s *.s


asmopt: $(HDR) $(SRC)
	$(CC) $(CFOPT) $(CFARCH) $(CFASM) -S $(SRC)
	rename .s _opt.s *.s
	rename _dbg_opt _dbg *.s


# clean up
clean:
	rm bindbg binopt *.s
