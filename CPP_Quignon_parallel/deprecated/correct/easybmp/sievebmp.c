#include "qdbmp.h"
#include "ds.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PALETTESIZE 255

BMP* bmp;
POINT max;

void sieve(long int end){
  int i;
  int j;
  POINT curr;
  
  for(i = 2; i < end; i++){
    curr.x=4;  
    curr.y=i;
    BMP_SetPixelIndex(bmp, curr.x, curr.y, 255 );
  }
  
  //all numbers:
  for(i = 2; i < end; i++){
    curr.x=4;
    //erase multiples of that number
    for(j=i+i; j< end; j=j+i){
      curr.y=j;
      BMP_SetPixelIndex(bmp, curr.x, curr.y, 0);
      //printf("%i\n", j);
    }
  }
}

//defines the POINT on the Framebuffer of the n th number
void spiral(long int n){
  POINT ret;
  long int dir, gone, dist, i;
  int circle;
  
  dist=0;
  dir=0;
  gone=0;
  
  ret.x=max.x/2;
  ret.y=max.y/2;
  
  //last corner before n
  for(gone = 0; dist<(n-gone); gone++){
    
    //increase square size every second iteration
    if((dir%2)==1){dist++;}
        
    //printf("dist = %i\n", dist);
    //printf("dir = %i\n", dir);
    
    //
    switch (dir) {
      case 1:
	for(i=0; i<dist; i++){
	  ret.x++;
	  gone++;
	  if(isprime(gone)) BMP_SetPixelIndex(bmp, ret.x, ret.y, 255 );
	}
	break;
      case 2:
	for(i=0; i<dist; i++){
	  ret.y++;
	  gone++;
	  if(isprime(gone)) BMP_SetPixelIndex(bmp, ret.x, ret.y, 255 );
	}
	break;
      case 3:
	for(i=0; i<dist; i++){
	  ret.x--;
	  gone++;
	  if(isprime(gone)) BMP_SetPixelIndex(bmp, ret.x, ret.y, 255 );
	}
	break;
      case 0:
	for(i=0; i<dist; i++){
	  ret.y--;
	  gone++;
	  if(isprime(gone)) BMP_SetPixelIndex(bmp, ret.x, ret.y, 255 );
	}
	break;
      default:
	break;
    }
    //drawpoint(ret, white);
    //printf("x = %i - y = %i\n", ret.x, ret.y);
    dir=(dir+1)%4;
    
  }
  
  //rest to reach n
  dist=n-gone;
  
  if(dist>0){
    switch (dir) {
      case 1:
	ret.x=ret.x+dist;
	break;
      case 2:
	ret.y=ret.y+dist;
	break;
      case 3:
	ret.x=ret.x-dist;
	break;
      case 0:
	ret.y=ret.y-dist;
	break;
      default:
	break;
    }
  }

}

//moves the given point in one direction
void step(POINT p, int n){
   switch (n%4) {
      case (0):
	p.y++;
	break;
      case (1):
	p.x++;
	break;
      case (2):
	p.y--;
	break;
      case (3):
	p.x--;
	break;
      default:
	break;
    }
}

// returns the next direction of the peano curve
int nextpeano(int n){
  if(n>9||n<0){return -1;}
  switch (n/9) {
    case (0):
    case (1):
    case (6):
    case (7):
      return 0;
    case (2):
    case (5):
      return 1;
    case (3):
    case (4):
      return 2;
    default:
      break;
    }  
}

int isprime(int number) {
    int i;
    if(number<=2){return 0;}
    for (i=2; i<number; i++) {
        if (number % i == 0 && i != number) return 0;
    }
    return 1;
}
int main( int argc, char* argv[] )
{
   POINT p;
   UINT i, j;
   
   max.x = atoi(argv[2]);
   max.y = atoi(argv[2]);
   
   /* Check arguments */
   if ( argc != 3 ){
      fprintf( stderr, "Usage: %s <input file> <width of the square BMP>\n", argv[ 0 ] );
      return 0;
   }
   
   /* create the BMP */
   bmp = BMP_Create( max.x, max.y, 8);
   
   /* The new image's palette - gradients from black to white  */
   for ( i = 0 ; i < PALETTESIZE ; ++i ){
      BMP_SetPaletteColor( bmp , i, i, i, i );
   }
   
   
   /* Iterate through all the image's pixels 
   for ( i = 0 ; i < max.x) ; ++i ){
      for ( j = 0 ; j < max.y) ; ++j ){
	 // Set RGB values 
	 BMP_SetPixelIndex( bmp, i, j, i+j );
      }
   }*/
   spiral(max.x*max.y);
   
   
   /* Save result */
   BMP_WriteFile( bmp, argv[1] );
   BMP_CHECK_ERROR( stdout, -2 );
   
   /* Free all memory allocated for the image */
   BMP_Free( bmp );
   return 0;
}
