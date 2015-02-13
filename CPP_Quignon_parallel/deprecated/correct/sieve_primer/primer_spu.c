#include<stdio.h>
//#include<libspe.h>
//#include<spu_mfcio.h>
#include <stdlib.h>
#include "ds.h"

//TODO: inefficient!
int isprime(int number) {
    int i;
    if(number<=2){return 0;}
    for (i=2; i<number; i++) {
        if (number % i == 0 && i != number) return 0;
    }
    return 1;
}



int main(unsigned long long speid,
	 unsigned long long argp,
	 unsigned long long envp
	 ) {
	int i;
	int* startaddr;
	int* curraddr;
	PRIMFIELD v;
	
	
	startaddr = malloc(8);
	if (startaddr == NULL)
	  return 1;
	curraddr=startaddr;
	v.bits=0000;
	
	//"generates primes"
	//8 bit primefield
	for(i=7; i<15; i++){
	  printf(" i = %i", i);
	 // printf("%i\n", v);
	  v.bits = v.bits << 1;
	  v.bits = v.bits | (isprime(i));
	  printf("\t %i\n", isprime(i));}
	
	printf("%i\n", v.bits);
	  //TODO: pointer arithmetic bitwise not bytewise
	  //curraddr++;
	  

	free(startaddr);
	
	return 0;
}
