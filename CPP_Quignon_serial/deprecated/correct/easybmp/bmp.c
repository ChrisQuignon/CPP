#include "qdbmp.h"
#include <stdio.h>
#include <stdlib.h>

#define PALETTESIZE 255

int main( int argc, char* argv[] )
{
	UCHAR	r, g, b;
	UINT	width, height;
	UINT	i, j;
	BMP*	bmp;

	/* Check arguments */
	if ( argc != 3 ){
	   fprintf( stderr, "Usage: %s <input file> <width of the square BMP>\n", argv[ 0 ] );
	   return 0;
	}
	 
	/* create the BMP */
	bmp = BMP_Create( atoi(argv[2]), atoi(argv[2]), 8);
	
	/* The new image's palette - gradients from black to white  */
	for ( i = 0 ; i < PALETTESIZE ; ++i ){
 	   BMP_SetPaletteColor( bmp , i, i, i, i );
	}
	
	/* Iterate through all the image's pixels */
	for ( i = 0 ; i < atoi(argv[2]) ; ++i ){
	   for ( j = 0 ; j < atoi(argv[2]) ; ++j ){
	    /* Set RGB values */
	    BMP_SetPixelIndex( bmp, i, j, i+j );
	   }
	}

	/* Save result */
	BMP_WriteFile( bmp, argv[1] );
	BMP_CHECK_ERROR( stdout, -2 );


	/* Free all memory allocated for the image */
	BMP_Free( bmp );

	return 0;
   }