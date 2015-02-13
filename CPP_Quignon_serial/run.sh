#!/bin/sh
clear
echo "Compiling:"
gcc bwbmp.c -Wall -pg -lm -lgmp bmpfile.h -o ulam.o
echo "creating Data:"
# 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16384 32768
  # 9 	 27 81     243 729      2187 6561      19683 59049
for i in 9 30 250 800 2100 #5250	
do
echo $i
./ulam.o data/ulam$i $i 1 > data/ulam$i.out
gprof ulam.o -b -Q > data/gprof_ulam$i.out 

done
echo "Data can be found in data/"
echo "Have a nice Day"