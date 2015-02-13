#include<stdio.h>
//#include<libspe.h>
#include<spu_mfcio.h>
#include <stdlib.h>
#include "ds.h"

STRUCTURE addr;

int main(unsigned long long speid,
	 unsigned long long argp,
	 unsigned long long envp
	 ) {
	int i;
	
	//get data
	mfc_get(&addr, argp, sizeof(PRIMEBITS), 0, 0, 0);
	mfc_write_tag_mask(1 << 0);
	mfc_read_tag_status_all();
	
	
	//TODO: Change size
	mfc_put(startaddr, addr.retaddr, sizeof(addr.length), 0, 0, 0);
	mfc_read_tag_status_all();
	
	free(startaddr);
	
	return 0;
}


//TODO: write other curves
POINT[] ecurve(PRIMFLIED p, int start) {
    int i;
    
    //TODO: implement position computation from start
    
}

