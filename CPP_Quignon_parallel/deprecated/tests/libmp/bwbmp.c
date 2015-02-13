#include "bmpfile.h"
#include <stdio.h>
#include <math.h>
#include <gmp.h>

#define PRIMEITS 6

typedef struct _POINT {
    uint32_t x; uint32_t y;
} POINT;

int max;
bmpfile_t *bmp;
rgb_pixel_t white = {255, 255, 255, 255};
rgb_pixel_t black = {0, 0, 0, 0};


/*
* Creates Ulam Spiral
* The Center Value is "start"
* The maximum X and Y Axis Values are "size"
*/
void spiral(unsigned long int size, mpz_t start){
   
   //Value of the current position
   mpz_t posi;
   
   // The current Point, where the spiral is
   POINT cur;
   
   // The direction the current spiral goes to
   // from 0 = right counter clockwise to 3 = right
   unsigned long int dir;
   
   //distance on X Axis to the center and corresponding counter
   unsigned long int dist, i;
  
   //The Size is the square of the maximum Axis Values
   size=size*size;
   
   //Initialisations
   mpz_init(posi);
   dist=0;
   dir=0;
   
   //Start at the center
   cur.x=max/2;
   cur.y=max/2;
   
   //Spiraling the complete Square
   while(size>0){
    
    //increase Distance after 2 Direction changes
    if((dir%2)==1){dist++;}
    
    //increase or decrease the Values on the Axis corresponding to the Direction
    switch (dir) {
      case 1:
	//on the complete range of the current square border:
	for(i=dist; i>0; i--){
	  cur.x++;
	  mpz_add_ui (posi, posi, 1);
	  
	  //if the Vlues is a prime(aproximation), set pixle color to white
	  if(mpz_probab_prime_p(posi, PRIMEITS)) bmp_set_pixel(bmp, cur.x, cur.y, white );
	  size--;
	}
	break;
      case 2:
	for(i=dist; i>0; i--){
	  cur.y++;
	  mpz_add_ui (posi, posi, 1);
	  if(mpz_probab_prime_p(posi, PRIMEITS)) bmp_set_pixel(bmp, cur.x, cur.y, white );
	   size--;
	}
	break;
      case 3:
	for(i=dist; i>0; i--){
	  cur.x--;
	  mpz_add_ui (posi, posi, 1);
	  if(mpz_probab_prime_p(posi, PRIMEITS)) bmp_set_pixel(bmp, cur.x, cur.y, white );
	   size--;
	}
	break;
      case 0:
	for(i=dist; i>0; i--){
	  cur.y--;
	  mpz_add_ui (posi, posi, 1);
	  if(mpz_probab_prime_p(posi, PRIMEITS)) bmp_set_pixel(bmp, cur.x, cur.y, white );
	   size--;
	}
	break;
      default:
	break;
    }
    dir=(dir+1)%4;
    
  }
  
  //clean up
  mpz_clear(posi);
}


int main( int argc, char* argv[] )
{
   
   //iteration Variables
   int i, j;
   
   //temporary File Stream
   FILE *stream;
   
   //First Number to start computation
   mpz_t start;
   
   //initialisations
   mpz_init(start);
   stream = tmpfile ();
   
   //maximum x and y Values of the BMP
   max = atoi(argv[2]);

   /* Check arguments */
  if (argc < 4) {
    printf("Usage: %s filename size startnumber.\n", argv[0]);
    return 1;
  }
  
  //create monochrom black BMP (default is white)
  bmp = bmp_create(max, max, 1);
  for(i =0; i< max; i++){
     for (j = 0; j<max;j++){
      bmp_set_pixel(bmp, i, j, black);
     }
  }
  
  //read invocation Values as mpz_t to basis 10
  fputs(argv[3], stream);
  mpz_inp_str(start, stream, 10);
  
  //start ulam spiral
  spiral(max, start);
  
  //save ulam spiral in bmp
  bmp_save(bmp, argv[1]);
  
  //clean up
  bmp_destroy(bmp);
  mpz_clear(start);
  fclose(stream);
  
  return 0;
}
   


