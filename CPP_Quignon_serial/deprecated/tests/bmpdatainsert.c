#include "qdbmp.h"
#include "ds.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>

#define PALETTESIZE 255

BMP* bmp;
POINT max;

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
   
   
   
   bmp.Data = 
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

   
   
   /* Save result */
   BMP_WriteFile( bmp, argv[1] );
   BMP_CHECK_ERROR( stdout, -2 );
   
   /* Free all memory allocated for the image */
   BMP_Free( bmp );
   return 0;
}
