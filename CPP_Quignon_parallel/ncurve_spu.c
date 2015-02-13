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
int blocks=0;
rgb_pixel_t * primz;

//distance on X Axis to the center and corresponding counter
unsigned long int dist, todo;
long int done;

//TODO: IMPLEMENT
int reload(){
  return 0;
}

void ncurve(unsigned long int lvl, unsigned int dir){
   if(lvl>0){
      switch(dir){	    
	 case TR:
	    ncurve(lvl-1, TR  ); cur.y++; //top
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, TL  ); cur.y++; //top
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, TR  ); cur.x++; //right
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, BR  ); cur.y--; //bottom
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, BL  ); cur.y--; //bottom
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, BR  ); cur.x++; //right
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, TR  ); cur.y++; //top
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, TL  ); cur.y++; //top
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, TR  );
	    break;
	 case TL:
	    ncurve(lvl-1, TL  ); cur.y++; //top
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, TR  ); cur.y++; //top
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, TL  ); cur.x--; //left
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, BL  ); cur.y--; //bottom
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, BR  ); cur.y--; //bottom
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, BL  ); cur.x--; //left
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, TL  ); cur.y++; //top
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, TR  ); cur.y++; //top
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, TL  );
	    break;
	 case BL:
	    ncurve(lvl-1, BL  ); cur.y--; //bottom
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, BR  ); cur.y--; //bottom
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, BL  ); cur.x--; //left
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, TL  ); cur.y++; //top
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, TR  ); cur.y++; //top
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, TL  ); cur.x--; //left
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, BL  ); cur.y--; //bottom
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, BR  ); cur.y--; //bottom
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, BL  );
	    break;
	 case BR:
	    ncurve(lvl-1, BR  ); cur.y--; //bottom
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, BL  ); cur.y--; //bottom
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, BR  ); cur.x++; //right
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, TR  ); cur.y++; //top
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, TL  ); cur.y++; //top
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, TR  ); cur.x++; //right
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, BR  ); cur.y--; //bottom 
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, BL  ); cur.y--; //bottom
	    done++;
	    if(done>=todo){while(reload()) sleep(SLEEPTIME);}
	    else {
	      if(primz[blocks].alpha) bmp_set_pixel(bmp, cur.y, cur.x, white);
	      else{
		bmp_set_pixel(bmp, cur.y, cur.x, black);
		break;}}
	    ncurve(lvl-1, BR  );
	    break;
	 default:
	    break;}}   
}


int main(unsigned long long speid,
	 unsigned long long argp,
	 unsigned long long envp
	 ) {
	int i;
	int next;
	long int count;
	
	//get BMPINIT
	mfc_get(&addr, argp, sizeof(BMPINIT), 0, 0, 0);
	mfc_write_tag_mask(1 << 0);
	mfc_read_tag_status_all();
	//mfc_read_tag_status_all();
	
	printf("\nNCURVE:\n");
	printf("size = %i \n", addr.size);
	printf("returnaddress = %p \n", addr.retaddr);
	printf("next = %p \n", addr.next);
	
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
	mfc_get(&primz, addr.next, sizeof(rgb_pixel_t), 0, 0, 0);
	mfc_write_tag_mask(1 << 0);
	mfc_read_tag_status_all();
	//mfc_read_tag_status_all();
	
	//INIT SPIRAL
	done=0;
	max=addr.size*addr.size;
	dist=0;
	dir=0;
   
	//Start at origin
	cur.x=0;
	cur.y=0;
	
	//loop until bmp is complete
	while (done<max){
	  
	  //get data
	  //get PRIMARRAY
	  mfc_get(&parray, next, sizeof(PRIMARRAY), 0, 0, 0);
	  mfc_write_tag_mask(1 << 0);
	  mfc_read_tag_status_all();
	  //mfc_read_tag_status_all(); 
	  mfc_get(&primz[blocks], addr.next, sizeof(rgb_pixel_t), 0, 0, 0);
	  mfc_write_tag_mask(1 << 0);
	  mfc_read_tag_status_all();
	  //mfc_read_tag_status_all();
	  
	  //check if new
	  if(count!=parray.count){
	    ncurve((int) (log(max)/log(3)), TR);
	    count=parray.count;
	    blocks++;
	  }
	}
	
	
	//writes back bmp
	mfc_put(bmp, addr.retaddr, bmp->dib.bmp_bytesz, 0, 0, 0);
	mfc_read_tag_status_all();
	
	return 0;
}

