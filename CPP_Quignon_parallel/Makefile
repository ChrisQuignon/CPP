# Makefile

CFLAGS= -pg
LIBS=bmpfile.h defs.c -lm
PROJNAME=ulam
 
all: gcc

spu: hilbert_spu.c spiral_spu.c ncurve_spu.c
	spu-gcc hilbert_spu.c $(CFLAGS) -o hilbert_spu.o $(LIBS)
	spu-gcc spiral_spu.c $(CFLAGS) -o spiral_spu.o $(LIBS)
	spu-gcc ncurve_spu.c $(CFLAGS) -o ncurve_spu.o $(LIBS)

ppu: spu
#hilbert
	ppu-embedspu hilbert_spu hilbert_spu.o hilbert_spu_csf.o 
#spiral
	ppu-embedspu spiral_spu spiral_spu.o spiral_spu_csf.o
#ncurve
	ppu-embedspu ncurve_spu ncurve_spu.o ncurve_spu_csf.o

gcc: ppu $(PROJNAME)_ppu.c
	gcc -m64 $(CFLAGS) $(LIBS) -lgmp -lspe $(PROJNAME)_ppu.c hilbert_spu_csf.o spiral_spu_csf.o ncurve_spu_csf.o  -o $(PROJNAME)_ppu.o

par: gcc
	./$(PROJNAME)_ppu.o data/$(PROJNAME) 1300 1

compile:
	gcc bwbmp.c $(CFLAGS) $(LIBS) -o $(PROJNAME).o
	
#do: 
#	./$(PROJNAME).o data/$(PROJNAME) 9 1 

clean:
	rm -f *.o
	rm -f *.bmp
	rm -f *.out
