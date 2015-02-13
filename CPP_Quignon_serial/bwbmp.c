#include "bmpfile.h"
#include "bmpfile.c"
#include <stdio.h>
#include <math.h>
#include <gmp.h>

#define PRIMEITS 6

#define BMPBITS 4

#define RIGHT 0
#define TOP 1
#define LEFT 2
#define BOTTOM 3

#define TR 0
#define TL 1
#define BL 2
#define BR 3

typedef struct _DIV  {
   unsigned int t;//numerator;
   unsigned int b;//denominator;
} DIV ;


typedef struct _POINT {
    uint32_t x; uint32_t y;
} POINT;

//Value of the current position
mpz_t posi;

// The current Point, where the spiral is
POINT cur;
bmpfile_t *bmp;
bmpfile_t *weights[8];
rgb_pixel_t white = {255, 255, 255, 0};
rgb_pixel_t black = {0, 0, 0, 0};
rgb_pixel_t red = {0, 255, 0, 255};

void markprime(mpz_t posi, bmpfile_t *bmp){
   if(mpz_probab_prime_p(posi, PRIMEITS)){
      bmp_set_pixel(bmp, cur.y, cur.x, red);
      //gmp_printf ("PRIME: mpz %Zd\n", posi);
   }
   //printf("posi: %i %i\n", cur.x, cur.y);
}

/*
* Creates Ulam Spiral
* The Center Value is "start"
* The maximum X and Y Axis Values are "size"
*/
void spiral(unsigned long int size, bmpfile_t *bmp){
   
   // The direction the current spiral goes to
   // from 0 = right counter clockwise to 3 = right
   unsigned long int dir;
   
   //distance on X Axis to the center and corresponding counter
   unsigned long int dist, i;
   
   //Initialisations
   dist=0;
   dir=0;
   
   //Axis length is the number of iteration *2 +1
   size=size*2+1;
   
   //Start at the center
   cur.x=size/2;
   cur.y=size/2;
   
   //The size is the square of the Axis length
   size=size*size;
   
   //Spiraling the complete Square
   while(size>0){
    
    //increase Distance after 2 Direction changes
    if((dir%2)==1){dist++;}
    //increase or decrease the Values on the Axis corresponding to the Direction
    switch (dir) {
      case 0:
	for(i=dist; i>0; i--){
	  cur.x++;
	  mpz_add_ui (posi, posi, 1);
	  markprime(posi, bmp);
	  size--;
	}
	break;
      case 1:
	//on the complete range of the current square border:
	for(i=dist; i>0; i--){
	  cur.y++;
	  mpz_add_ui (posi, posi, 1);
	  
	  //if the Values is a prime(aproximation), set pixle color to white
	  markprime(posi, bmp);
	  size--;
	}
	break;
      case 2:
	for(i=dist; i>0; i--){
	  cur.x--;
	  mpz_add_ui (posi, posi, 1);
	  markprime(posi, bmp);
	  size--;
	}
	break;
      case 3:
	for(i=dist; i>0; i--){
	  cur.y--;
	  mpz_add_ui (posi, posi, 1);
	  markprime(posi, bmp);
	   size--;
	}
	break;
      default:
	break;
    }
    dir=(dir+1)%4;}}

/*
* Creates Ulam hilbert curve
* The starting Value is "start"
* The number of iterations is lvl
*/
void hilbert(unsigned long int lvl, unsigned int dir, bmpfile_t *bmp){
   //gmp_printf ("Value: mpz %Zd\n", posi);
   //printf("posi: %i - %i\n", cur.x, cur.y);
   if(lvl>0){
      switch(dir){
	    
	 case RIGHT:
	    
	    hilbert(lvl-1, TOP, bmp); 
	    
	    cur.x++; //right
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    
	    hilbert(lvl-1, RIGHT, bmp);
	    
	    cur.y++; //top
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    
	    hilbert(lvl-1, RIGHT, bmp); 
	    
	    cur.x--; //left
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    
	    hilbert(lvl-1, BOTTOM, bmp);
	    
	    break;
	    
	 case TOP:
	    hilbert(lvl-1, RIGHT, bmp);
	    
	    cur.y++; //top
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    
	    hilbert(lvl-1, TOP, bmp);
	    
	    cur.x++; //right
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    
	    hilbert(lvl-1, TOP, bmp);
	    
	    cur.y--; //down
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    
	    hilbert(lvl-1, LEFT, bmp);
	    break;
	    

	 case LEFT:
	    hilbert(lvl-1, BOTTOM, bmp);
	    
	    cur.x--; //left
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    
	    hilbert(lvl-1, LEFT, bmp); 
	    
	    cur.y--; //down
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    
	    hilbert(lvl-1, LEFT, bmp);
	    
	    cur.x++;//right
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    
	    hilbert(lvl-1, TOP, bmp);
	    break;
	    
	 case BOTTOM:
	    hilbert(lvl-1, LEFT, bmp);
	    
	    cur.y--; //down
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    
	    hilbert(lvl-1, BOTTOM, bmp);
	    
	    cur.x--; //left
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    
	    hilbert(lvl-1, BOTTOM, bmp);
	    
	    cur.y++;//up
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    
	    hilbert(lvl-1, RIGHT, bmp);
	    break;

	 default:
	    break; }}}

void ncurve(unsigned long int lvl, unsigned int dir, bmpfile_t *bmp){
   if(lvl>0){
      switch(dir){	    
	 case TR:
	    ncurve(lvl-1, TR, bmp); cur.y++; //top
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, TL, bmp); cur.y++; //top
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, TR, bmp); cur.x++; //right
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, BR, bmp); cur.y--; //bottom
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, BL, bmp); cur.y--; //bottom
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, BR, bmp); cur.x++; //right
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, TR, bmp); cur.y++; //top
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, TL, bmp); cur.y++; //top
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, TR, bmp);
	    break;
	 case TL:
	    ncurve(lvl-1, TL, bmp); cur.y++; //top
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, TR, bmp); cur.y++; //top
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, TL, bmp); cur.x--; //left
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, BL, bmp); cur.y--; //bottom
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, BR, bmp); cur.y--; //bottom
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, BL, bmp); cur.x--; //left
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, TL, bmp); cur.y++; //top
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, TR, bmp); cur.y++; //top
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, TL, bmp);
	    break;
	 case BL:
	    ncurve(lvl-1, BL, bmp); cur.y--; //bottom
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, BR, bmp); cur.y--; //bottom
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, BL, bmp); cur.x--; //left
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, TL, bmp); cur.y++; //top
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, TR, bmp); cur.y++; //top
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, TL, bmp); cur.x--; //left
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, BL, bmp); cur.y--; //bottom
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, BR, bmp); cur.y--; //bottom
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, BL, bmp);
	    break;
	 case BR:
	    ncurve(lvl-1, BR, bmp); cur.y--; //bottom
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, BL, bmp); cur.y--; //bottom
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, BR, bmp); cur.x++; //right
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, TR, bmp); cur.y++; //top
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, TL, bmp); cur.y++; //top
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, TR, bmp); cur.x++; //right
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, BR, bmp); cur.y--; //bottom 
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, BL, bmp); cur.y--; //bottom
	    mpz_add_ui (posi, posi, 1);
	    markprime(posi, bmp);
	    ncurve(lvl-1, BR, bmp);
	    break;
	 default:
	    break;}}   
}

DIV *prob0deg(DIV *prob, bmpfile_t *bmp){
   int i, j;
   for(i =0; i < bmp->dib.height; i++){
      for (j =0; j < bmp->dib.width; j++){
	 if(bmp->pixels[j][i].alpha%2) prob[i].t++;}
      prob[i].b=j;}
   return prob;}

DIV *prob135deg(DIV *prob, bmpfile_t *bmp){
   int i, j;
   for(i=bmp->dib.height; i>0;  i--){
      for(j=0; j < (bmp->dib.width-i); j++){
	 if(bmp->pixels[j][i+j].alpha%2) prob[bmp->dib.height-i].t++;}
      prob[bmp->dib.height-i].b=j;}
   for(j=0; j < bmp->dib.width;  j++){
      for(i=0; i < (bmp->dib.width-j); i++){
	 if(bmp->pixels[j+i][i].alpha%2) prob[bmp->dib.height+j].t++;}
      prob[bmp->dib.height+j].b=i;}
   return prob;}

DIV *prob90deg(DIV *prob, bmpfile_t *bmp){
   int i, j;
   for(i =0; i < bmp->dib.width; i++){
      for (j =0; j < bmp->dib.height; j++){
	 if(bmp->pixels[i][j].alpha%2) prob[i].t++;}
      prob[i].b=bmp->dib.height;}
   return prob;}

DIV *prob45deg(DIV *prob, bmpfile_t *bmp){
   int i, j;
   //45°
   //printf("\n45°:\n");
   for(j=bmp->dib.width; j > 0 ; j--){
      for(i=0; i < bmp->dib.height-j;  i++){
	 if(bmp->pixels[bmp->dib.width-i-1][i+j].alpha%2) prob[bmp->dib.width-j].t++;}
      prob[bmp->dib.width-j].b=i;}
    for(j=bmp->dib.width; j > 1 ; j--){      
 	 for(i=0; i < j;  i++){       
 	    if(bmp->pixels[j-i-1][i].alpha%2) prob[bmp->dib.height+bmp->dib.width-j].t++;}
 	 prob[bmp->dib.height+bmp->dib.width-j].b=i;}
   return prob;}
/*
void col0deg(DIV *prob, bmpfile_t *bmp){ 
   int i, j;
   //horizontal
   for(i =0; i < bmp->dib.height; i++){
      for (j =0; j < bmp->dib.width; j++){
//	 printf("%f = %f, %i\n", prob[i], 255*prob[i], (int)(255*prob[i]));
	 bmp->pixels[j][i].red=(int)(255*prob[i]);
	 bmp->pixels[j][i].green=(int)(255*prob[i]);
	 bmp->pixels[j][i].blue=(int)(255*prob[i]);}}}

void col135deg(DIV *prob, bmpfile_t *bmp){
   int i, j;
   for(i=bmp->dib.height; i>0;  i--){
      for(j=0; j < (bmp->dib.width-i); j++){
	 bmp->pixels[j][i+j].red=(int)(255*prob[bmp->dib.height-i]);
	 bmp->pixels[j][i+j].green=(int)(255*prob[bmp->dib.height-i]);
	 bmp->pixels[j][i+j].blue=(int)(255*prob[bmp->dib.height-i]);}}
   for(j=0; j < bmp->dib.width;  j++){
      for(i=0; i < (bmp->dib.width-j); i++){
	 bmp->pixels[j+i][i].red=(int)(255*prob[bmp->dib.height+j]);
	 bmp->pixels[j+i][i].green=(int)(255*prob[bmp->dib.height+j]);
	 bmp->pixels[j+i][i].blue=(int)(255*prob[bmp->dib.height+j]);}}}

void col90deg(DIV *prob, bmpfile_t *bmp){
   int i, j;
   //vertical
//    printf("\nvertical: \n");
   for(i =0; i < bmp->dib.width; i++){
      for (j =0; j < bmp->dib.height; j++){
	 bmp->pixels[i][j].red=(int)(255*prob[i]);
	 bmp->pixels[i][j].green=(int)(255*prob[i]);
	 bmp->pixels[i][j].blue=(int)(255*prob[i]);}}}

void col45deg(DIV *prob, bmpfile_t *bmp){
   int i, j;
   for(j=bmp->dib.width; j > 0 ; j--){
      for(i=0; i < bmp->dib.height-j;  i++){
	 bmp->pixels[bmp->dib.width-i-1][i+j].blue=(int)(255*prob[bmp->dib.width-j]);
	 bmp->pixels[bmp->dib.width-i-1][i+j].green=(int)(255*prob[bmp->dib.width-j]);
	 bmp->pixels[bmp->dib.width-i-1][i+j].red=(int)(255*prob[bmp->dib.width-j]);}}
    for(j=bmp->dib.width; j > 1 ; j--){
 	 for(i=0; i < j;  i++){
	    bmp->pixels[j-i-1][i].blue=(int)(255*prob[bmp->dib.height+bmp->dib.width-j]);
	    bmp->pixels[j-i-1][i].green=(int)(255*prob[bmp->dib.height+bmp->dib.width-j]);
 	    bmp->pixels[j-i-1][i].red=(int)(255*prob[bmp->dib.height+bmp->dib.width-j]);}}}
*/

void probs(){
   DIV * deg0   = calloc(bmp->dib.height, sizeof(DIV));
   DIV * deg135 = calloc((bmp->dib.height+bmp->dib.width-1), sizeof(DIV));
   DIV * deg90  = calloc(bmp->dib.width, sizeof(DIV));
   DIV * deg45  = calloc((bmp->dib.width+bmp->dib.height-1), sizeof(DIV));
   int i;
   
   //all 135° lines 
   prob0deg(deg0, bmp);
   prob135deg(deg135, bmp);
   prob90deg(deg90, bmp);
   prob45deg(deg45, bmp);
   
   /**/
   printf("\n0 degrees: \n");
   for(i = 0; i< bmp->dib.height ; i++){printf("%i/%i \n", deg0[i].t,  deg0[i].b);}
   
   printf("\n45 degrees: \n");
   for(i = 0; i< bmp->dib.width+bmp->dib.height-1 ; i++){printf("%i/%i \n", deg45[i].t,  deg45[i].b);}
   
   printf("\n90 degrees: \n");
   for(i = 0; i< bmp->dib.width ; i++){printf("%i/%i \n", deg90[i].t,  deg90[i].b);}
   
   printf("\n135 degrees: \n");
   for(i = 0; i< bmp->dib.height+bmp->dib.width-1 ; i++){printf("%i/%i \n", deg135[i].t,  deg135[i].b);}
   
     
   //free(deg0);
   //free(deg135);
   //free(deg90);
   //free(deg45);
}

int main( int argc, char* argv[] ){
  //iteration Variables
  int i, j, k, its, ith, itn;
  mpz_init(posi);
  char file[256];
  DIV d;
  /* Check arguments */
  if (argc < 4) {
    printf("Usage: %s filename length start.\n", argv[0]);
    return 1;
  } 
  
  printf("length = %i\n", atoi(argv[2]));
  
  //function that normalize the wished bmp border length to the algorithm
  ith = (int) log2((double) (atoi(argv[2])));
  printf("ith = %i\n", ith);

  its = atoi(argv[2])/2;
  printf("its = %i\n", its);
  
  //log to base 3
  itn = (int) (log((double) (atoi(argv[2])))/log(3));
  printf("itn = %i\n", itn);
  
  //read invocation Values as mpz_t to basis 10
  mpz_set_str (posi, argv[3], 10);
  //gmp_printf ("posi: mpz %Zd\n", posi);
  
  
  /*HILBERT BLOCK:*/
   
   //create black BMP (default is white)
   bmp = bmp_create(pow(2, ith), pow(2, ith), BMPBITS);
   for(i =0; i< bmp_get_width(bmp); i++){
     for (j = 0; j<bmp_get_height(bmp);j++){
      bmp_set_pixel(bmp, i, j, black);
     }
   }

   //start point is lower left corner
   cur.x=0;
   cur.y=0;
   mpz_set_str (posi, argv[3], 10);
  
   //start ulam hilbert curve
   hilbert(ith, TOP, bmp);
   probs();
   
   //save bmp
   strcpy(file, argv[1]);
   strcat(file, "_HILBERT.bmp");
   //very time intensive!
   //bmp_save(bmp, file);
  //HILBERT BLOCK END


  /*ULAM BLOCK*/
   //create black BMP (default is white)
   bmp = bmp_create((its*2+1), (its*2+1), BMPBITS);
   for(i =0; i< bmp_get_width(bmp); i++){
     for (j = 0; j<bmp_get_height(bmp);j++){
      bmp_set_pixel(bmp, i, j, black);
     }
   }
   
   mpz_set_str (posi, argv[3], 10);
   
   spiral(its, bmp);
   probs();

   //save bmp
   strcpy(file, argv[1]);
   strcat(file, "_SPIRAL.bmp");
   //bmp_save(bmp, file);
  //UAM BLOCK END
  
  
  /*N CURVE BLOCK*/
   //create black BMP (default is white)
   bmp = bmp_create(pow(3, itn), pow(3, itn), BMPBITS);
   for(i =0; i< bmp_get_width(bmp); i++){
     for (j = 0; j<bmp_get_height(bmp);j++){
      bmp_set_pixel(bmp, i, j, black);
     }
   }
   
   cur.x=0;
   cur.y=0;
   mpz_set_str (posi, argv[3], 10);
   
   ncurve(itn, TR, bmp);
   probs();

   //save bmp
   strcpy(file, argv[1]);
   strcat(file, "_N_CURVE.bmp");
   //bmp_save(bmp, file);
  //N CURVE BLOCK END
  
  /*
  col0deg(deg135, bmp);
  col45deg(deg135, bmp);
  col90deg(deg135, bmp);
  col135deg(deg135, bmp);*/
  
   
  //clean up
  bmp_destroy(bmp);
  mpz_clear(posi);
  //free(deg0);
  //free(deg135);
  //free(deg90);
  //free(deg45);
  
  return 0;
}
