# Makefile

CFLAGS=-Wall -pg
LIBS=-lm -lgmp bmpfile.h
 
all: compile

#spu: $(PROJNAME)_spu.c
#	spu-gcc $(PROJNAME)_spu.c $(CFLAGS) -o $(PROJNAME)_spu

#ppu: spu
#	ppu-embedspu $(CFLAGS) $(PROJNAME)_spu $(PROJNAME)_spu $(PROJNAME)_spu_csf.o
#	
compile:
	gcc bwbmp.c $(CFLAGS) $(LIBS) -o ulam.o
	
do: 
	./ulam.o data/ulam 9 1 

clean:
	rm -f ulam.o
	rm -f *.bmp
	rm -f *.out
