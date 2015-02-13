#include "bmpfile.h"
#include "bmpfile.c"
#include <stdio.h>
#include <math.h>
#include "defs.c"
#include <spu_mfcio.h>
#include <stdlib.h>
#include <unistd.h>


// The current Point, where the spiral is
POINT cur;

//bmpfile_t *bmp;
rgb_pixel_t ** bmppix __attribute__((aligned(128)));
rgb_pixel_t white = {255, 255, 255, 2};
rgb_pixel_t black = {0, 0, 0, 0};
rgb_pixel_t * * pixaddrs __attribute__((aligned(128)));;
rgb_pixel_t * * ret __attribute__((aligned(128)));
rgb_pixel_t * * input;
long int count;
// The direction the current spiral goes to
// from 0 = right counter clockwise to 3 = right
unsigned long int dir;
unsigned long int max;
rgb_pixel_t * inpix;

//distance on X Axis to the center and corresponding counter
unsigned long int dist, todo;
long int done;

// function that reloads data
void reload(INIT * addr){
  mfc_get(addr, addr->this, sizeof(BMPINIT), 0, 0, 0);
  mfc_read_tag_status_all();

  // if the needed data is updated: 
  if(count<=addr->count) {
    //get Data
    mfc_get(inpix, (int) pixaddrs[count], addr->primzsize*(sizeof(rgb_pixel_t)), 0, 0, 0);
    mfc_read_tag_status_all();
    
    count++;
    done=0;
    todo=addr->primzsize; 
  }
  else reload(addr);	// if not: try again (sleep();) would kill the thread
}

/*
* Creates Ulam hilbert curve
* The starting Value is "start"
* The number of iterations is lvl
* the algorithm is described in the report
*/
void hilbert(long int lvl, unsigned int dir, INIT * addr){
   //printf("lvl: %i   ", lvl);
   if(lvl>0){
     //printf("posi= %i %i\t", cur.x, cur.y);
     //printf("done = %i\n", done);
      switch(dir){
	    
	 case RIGHT:
	    hilbert(lvl-1, TOP, addr); 
	    
	    cur.x++; //right
	    done++;
	    if(done>=todo) reload(addr);
	    else {
	      if(inpix[done].alpha) bmppix[cur.x][cur.y]=white;
	      else bmppix[cur.x][cur.y]=black;}
	      
	    hilbert(lvl-1, RIGHT, addr);
	    
	    cur.y++; //top
	    done++;
	    if(done>=todo) reload(addr);
	    else {
	      if(inpix[done].alpha) bmppix[cur.x][cur.y]=white;
	      else bmppix[cur.x][cur.y]=black;}
	    hilbert(lvl-1, RIGHT, addr); 
	    
	    cur.x--; //left
	    done++;
	    if(done>=todo) reload(addr);
	    else {
	      if(inpix[done].alpha) bmppix[cur.x][cur.y]=white;
	      else bmppix[cur.x][cur.y]=black;}
	    hilbert(lvl-1, BOTTOM, addr);
	    
	    break;
	    
	 case TOP:
	    hilbert(lvl-1, RIGHT, addr);
	    
	    cur.y++; //top
	    done++;
	    if(done>=todo) reload(addr);
	    else {
	      if(inpix[done].alpha) bmppix[cur.x][cur.y]=white;
	      else bmppix[cur.x][cur.y]=black;}
	    hilbert(lvl-1, TOP, addr);
	    
	    cur.x++; //right
	    done++;
	    if(done>=todo) reload(addr);
	    else {
	      if(inpix[done].alpha) bmppix[cur.x][cur.y]=white;
	      else bmppix[cur.x][cur.y]=black;}
	    hilbert(lvl-1, TOP, addr);
	    
	    cur.y--; //down
	    done++;
	    if(done>=todo) reload(addr);
	    else {
	      if(inpix[done].alpha) bmppix[cur.x][cur.y]=white;
	      else bmppix[cur.x][cur.y]=black;}
	    hilbert(lvl-1, LEFT, addr);
	    break;
	    

	 case LEFT:
	    hilbert(lvl-1, BOTTOM, addr);
	    
	    cur.x--; //left
	    done++;
	    if(done>=todo) reload(addr);
	    else {
	      if(inpix[done].alpha) bmppix[cur.x][cur.y]=white;
	      else bmppix[cur.x][cur.y]=black;}
	    hilbert(lvl-1, LEFT, addr); 
	    
	    cur.y--; //down
	    done++;
	    if(done>=todo) reload(addr);
	    else {
	      if(inpix[done].alpha) bmppix[cur.x][cur.y]=white;
	      else bmppix[cur.x][cur.y]=black;}
	    hilbert(lvl-1, LEFT, addr);
	    
	    cur.x++;//right
	    done++;
	    if(done>=todo) reload(addr);
	    else {
	      if(inpix[done].alpha) bmppix[cur.x][cur.y]=white;
	      else bmppix[cur.x][cur.y]=black;}
	    hilbert(lvl-1, TOP, addr);
	    break;
	    
	 case BOTTOM:
	    hilbert(lvl-1, LEFT, addr);
	    
	    cur.y--; //down
	    done++;
	    if(done>=todo) reload(addr);
	    else {
	      if(inpix[done].alpha) bmppix[cur.x][cur.y]=white;
	      else bmppix[cur.x][cur.y]=black;}
	    hilbert(lvl-1, BOTTOM, addr);
	    
	    cur.x--; //left
	    done++;
	    if(done>=todo) reload(addr);
	    else {
	      if(inpix[done].alpha) bmppix[cur.x][cur.y]=white;
	      else bmppix[cur.x][cur.y]=black;}
	    hilbert(lvl-1, BOTTOM, addr);
	    
	    cur.y++;//up
	    done++;
	    if(done>=todo) reload(addr);
	    else {
	      if(inpix[done].alpha) bmppix[cur.x][cur.y]=white;
	      else bmppix[cur.x][cur.y]=black;}
	    
	    hilbert(lvl-1, RIGHT, addr);
	    break;

	 default:
	    break; }}}

/* function to put an arbitrary size of data
*  mfc_put is restricted by size
*  this function splits the data recursively
*  until it can be transefer by mfc_put
*/ 
void putback(uint64_t what, uint64_t where, long int size){
  if(size<=4000){
    mfc_put(what, where, size, 0, 0, 0);
    mfc_read_tag_status_all();}
  else {
    putback(what, where , size/2);
    putback((what)+size/2, (where)+size/2 , size/2);}
}

/* function to get arbitrary large data - analogue to putback();*/
void getfrom(uint64_t what, uint64_t where, long int size){
  if(size<=4000){
    mfc_get(what, where, size, 0, 0, 0);
    mfc_read_tag_status_all();
  else {
    getfrom(what, where , size/2);
    getfrom((what)+size/2, (where)+size/2 , size/2);}
}


int main(unsigned long long speid,
	 unsigned long long argp,
	 unsigned long long envp
	 ) {
	INIT * addr __attribute__((aligned(128)));	//inititation struct
	int i, j;	//iteration variables
	
	//tag mask for mfc_ - it the same for every cal of this SPU
	mfc_write_tag_mask(1<<0);

	//get initiation struct
	addr=calloc(1, sizeof(INIT));
	mfc_get(addr, argp, 128, 0, 0, 0);
	mfc_read_tag_status_all();
	
	/*debugging output
	printf("\nHILBERT:\n");
	printf("count = %i \n", addr->count);
	printf("bmpsize = %i \n", addr->bmpsize);
	printf("primzsize = %i \n", addr->primzsize);
	printf("pixaddr = %i \n", (int)addr->pixaddr);
	printf("this = %i \n", (int)addr->this);
	printf("retaddr = %i \n", (int)addr->retaddr);*/
	
	/*create pixelarray that will be returned*/
	bmppix=calloc(addr->bmpsize, sizeof(rgb_pixel_t *));
	for(i=0; i<addr->bmpsize; i++) {
	  bmppix[i]=malloc(addr->bmpsize*sizeof(rgb_pixel_t));}
	
	//pixaddr initialisiation
	pixaddrs=calloc(addr->bmpsize*addr->bmpsize/addr->primzsize, sizeof(rgb_pixel_t *));
	getfrom(pixaddrs
	       , addr->pixaddr
	       , addr->bmpsize*addr->bmpsize/addr->primzsize*(sizeof(rgb_pixel_t *)));
	

	/*TODO: array initialisation
	inpix = calloc(addr->primzsize, sizeof(rgb_pixel_t));
	mfc_get(inpix, (int)&pixaddrs[0], addr->primzsize*(sizeof(rgb_pixel_t)), 0, 0, 0);
	mfc_read_tag_status_any();
	*/
	
	// set starting values
	todo=addr->primzsize;
	count=1;

	done=0;
	dist=0;
	dir=0;

	cur.x=0;
	cur.y=0;

	//invoce alignment algorithm
	hilbert((int)(log((int)addr->bmpsize)/log(2)), TOP, addr);

	//return the traversed data:

	//get the first of the 2 dimensional array to where the data should be returned:
	ret=calloc(addr->bmpsize, sizeof(rgb_pixel_t *));
	for(i=0; i<  addr->retaddr; i++){
	  mfc_get(ret[i], (int)addr->retaddr, sizeof(rgb_pixel_t *), 0, 0, 0);
	  mfc_read_tag_status_all();}
	
	//return the data
	for (i=0; i< addr->bmpsize; i++)
	  putback(&bmppix[i][0], ret[i], (addr->retaddr)*sizeof(rgb_pixel_t));}
	mfc_read_tag_status_all();

	return 0;
}

