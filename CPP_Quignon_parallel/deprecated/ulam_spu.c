#include<stdio.h>
//#include<libspe.h>
#include<spu_mfcio.h>
#include <stdlib.h>

#define MAX 4000

//alignment has to be outside the main
typedef struct _RANDARRAY {
	    int seed;
	    int max;
	    int retaddr;
	    int noop;
	} RANDARRAY __attribute__((aligned(16)));
	
	RANDARRAY addr;
	int rnds[MAX] __attribute__((aligned(128)));

int main(unsigned long long speid,
	 unsigned long long argp,
	 unsigned long long envp
	 ) {
	int i;
	//get data
	mfc_get(&addr, argp, sizeof(RANDARRAY), 0, 0, 0);
	mfc_write_tag_mask(1 << 0);
	mfc_read_tag_status_all();
	//mfc_get(&s, addr.seed, sizeof(s), 0, 0, 0);
	//mfc_get(&m, addr.max , sizeof(m), 0, 0, 0);
	//mfc_get(&r, addr.retaddr , sizeof(int), 0, 0, 0);
	//mfc_read_tag_status_all();
	
	//printf("seed = %i \n", addr.seed);
	//printf("max = %i \n", addr.max);
	//printf("AND NOW:\n");
	//printf("retaddr = %p \n", addr.retaddr);
	//printf("&addr.retaddr = %p \n", &addr.retaddr);
	//printf("IM HERE");
	//if(addr.max>1028){return -1;}
	rnds[0]=addr.seed;
	
	//"randomize"
	
	for (i=1; i<addr.max; i++){
	  rnds[i] = (rnds[i-1]*3)%addr.max;
	  printf(" %i\t", rnds[i]);
	  //write until duplicate is created
	  if (rnds[0]==rnds[i]) {
	    rnds[i]=0;
	    break;}
	}
	
	//writes back array[1024] to array[max] - baaad!
	mfc_put(rnds, addr.retaddr, sizeof(int[addr.max]), 0, 0, 0);
	mfc_read_tag_status_all();
	
	return 0;
}
