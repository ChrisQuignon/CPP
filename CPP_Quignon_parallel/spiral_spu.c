#include "bmpfile.h"
#include "bmpfile.c"
#include <stdio.h>
#include <math.h>
#include "defs.c"
//#include<libspe.h>
#include<spu_mfcio.h>
#include <stdlib.h>
#include <unistd.h>

/*
*THIS FILE IS NOT IMPLEMENTED PROPERLY
*IT DOES NOT REFLECT THE LAST IMPLEMENTATION
*FOR THIS, PLEASE HAVE A LOOK AT
* hilbert_spu.c
*/

//alignment has to be outside the main
BMPINIT addr;
PRIMARRAY parray;

// The current Point, where the spiral is
POINT cur;
bmpfile_t *bmp;
rgb_pixel_t white = {255, 255, 255, 0};
rgb_pixel_t black = {0, 0, 0, 0};

// The direction the current spiral goes to
// from 0 = right counter clockwise to 3 = right
unsigned long int dir;
unsigned long int max;
rgb_pixel_t * primz;

//distance on X Axis to the center and corresponding counter
unsigned long int dist, todo;
long int done;
int blocks =0;

/*
* Creates Ulam Spiral
* The Center Value is "start"
* The maximum X and Y Axis Values are "size"
*/
void spiral(int to){
   
   //Initialisations
   unsigned long int i;
   
   //Spiraling the complete Square
   while(todo>0){
    
    //increase Distance after 2 Direction changes
    if((dir%2)==1){dist++;}
    //increase or decrease the Values on the Axis corresponding to the Direction
    switch (dir) {
      case 0:
	for(i=dist; i>0; i--){
	  cur.x++;
	  done++;
	  if(primz[blocks].alpha){
	    bmp_set_pixel(bmp, cur.y, cur.x, white);
	  }
	  else{
	    bmp_set_pixel(bmp, cur.y, cur.x, black);
	    break;
	  }
	  todo--;
	}
	break;
      case 1:
	//on the complete range of the current square border:
	for(i=dist; i>0; i--){
	  cur.y++;
	  done++;
	  //if the Values is a prime(aproximation), set pixle color to white
	  	  if(primz[blocks].alpha){
	    bmp_set_pixel(bmp, cur.y, cur.x, white);
	  }
	  else{
	    bmp_set_pixel(bmp, cur.y, cur.x, black);
	    break;
	  }
	  todo--;
	}
	break;
      case 2:
	for(i=dist; i>0; i--){
	  cur.x--;
	  done++;
	  if(primz[blocks].alpha){
	    bmp_set_pixel(bmp, cur.y, cur.x, white);
	  }
	  else{
	    bmp_set_pixel(bmp, cur.y, cur.x, black);
	    break;
	  }
	  todo--;
	}
	break;
      case 3:
	for(i=dist; i>0; i--){
	  cur.y--;
	  done++;
	  if(primz[blocks].alpha){
	    bmp_set_pixel(bmp, cur.y, cur.x, white);
	  }
	  else{
	    bmp_set_pixel(bmp, cur.y, cur.x, black);
	    break;
	  }
	  todo--;
	}
	break;
      default:
	break;
    }
    dir=(dir+1)%4;}}


int main(unsigned long long speid,
	 unsigned long long argp,
	 unsigned long long envp
	 ) {
	int i;
	int next;
	int count;
	rgb_pixel_t * primz;
	
	//get BMPINIT
	mfc_get(&addr, argp, sizeof(BMPINIT), 0, 0, 0);
	mfc_write_tag_mask(1 << 0);
	mfc_read_tag_status_all();
	//mfc_read_tag_status_all();
	
	printf("\nSPIRAL:\n");
	printf("size = %i \n", addr.size);
	printf("returnaddress = %i \n", addr.retaddr);
	printf("next = %i \n", addr.next);
	
	next=addr.next;
	
	//CREATE BMP
	//bmp = bmp_create(addr.size, addr.size, addr.bmpbits);
	
	//array initialisation
	mfc_get(&parray, next, sizeof(PRIMARRAY), 0, 0, 0);
	mfc_write_tag_mask(1 << 0);
	mfc_read_tag_status_all();
	
	todo=parray.size;
	next=parray.next;
	count=parray.count;
	
	//mfc_read_tag_status_all(); 
	mfc_get(&primz, next, sizeof(rgb_pixel_t), 0, 0, 0);
	mfc_write_tag_mask(1 << 0);
	mfc_read_tag_status_all();
	//mfc_read_tag_status_all();
	
	//INIT SPIRAL
	done=0;
	max=addr.size*addr.size;
	dist=0;
	dir=0;
   
	//Start at the center
	cur.x=addr.size/2;
	cur.y=addr.size/2;
	
	//loop until bmp is complete
	while (done<max){
	  
	  //get data
	  //get PRIMARRAY
	  mfc_get(&parray, next, sizeof(PRIMARRAY), 0, 0, 0);
	  mfc_write_tag_mask(1 << 0);
	  mfc_read_tag_status_all();
	  //mfc_read_tag_status_all(); 
	  mfc_get(&primz[blocks], next, sizeof(rgb_pixel_t), 0, 0, 0);
	  mfc_write_tag_mask(1 << 0);
	  mfc_read_tag_status_all();
	  //mfc_read_tag_status_all();
	  
	  //check if new
	  if(count!=parray.count){
	    spiral(parray.size);
	    count=parray.count;
	    blocks++;
	  }
	}
	
	//writes back bmp
	mfc_put(bmp, addr.retaddr, 40+bmp->dib.bmp_bytesz, 0, 0, 0);
	mfc_read_tag_status_all();
	
	return 0;
}

