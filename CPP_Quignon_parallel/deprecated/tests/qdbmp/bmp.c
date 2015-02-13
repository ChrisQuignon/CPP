#include <stdio.h>
#include "qdbmp.h"

int main( int argc, const char* argv[] ){
   BMP* bmp;
   BMP_STATUS s;
   int i;
   
   //BMP_Create( UINT width, UINT height, USHORT depth )
   bmp = BMP_Create(400, 400, 1);
   s = BMP_GetError();
   printf("%s", BMP_GetErrorDescription());
   
   BMP_SetPaletteColor( bmp, 0, 0x00, 0x00, 0x00 );
   BMP_SetPaletteColor( bmp, 1, 0x11, 0x11, 0x11 );
   
   for(i =1; i<400; i++){
       BMP_SetPixelIndex( bmp, i, i, 0 );
      
   }
   
   
   BMP_WriteFile(bmp, argv[1]);
   BMP_Free(bmp);
   //printf( "arg: %s\n", argv[1] );
   
   
   
}

