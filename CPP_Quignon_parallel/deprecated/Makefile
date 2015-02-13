# Makefile

CFLAGS=-Wall
PROJNAME=ulam

all: execute

spu: $(PROJNAME)_spu.c
	spu-gcc $(PROJNAME)_spu.c $(CFLAGS) -o $(PROJNAME)_spu

ppu: spu
	ppu-embedspu $(CFLAGS) $(PROJNAME)_spu $(PROJNAME)_spu $(PROJNAME)_spu_csf.o
	
gcc: ppu $(PROJNAME)_ppu.c
	gcc -m64 $(PROJNAME)_ppu.c $(PROJNAME)_spu_csf.o $(CFLAGS) -lspe -o $(PROJNAME)_ppu $(CFLAGS)
	
execute: gcc $(PROJNAME)_spu_csf.o
	./$(PROJNAME)_ppu

clean:
	rm -f $(PROJNAME)_ppu
	rm -f $(PROJNAME)_spu
	rm -f $(PROJNAME)_spu_csf.o
