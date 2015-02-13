#include <stdlib.h>
#include <string.h>

int main( int argc, const char* argv[] ){
   
   
   return 0;
}

/* Bitmap header */
typedef struct _FILE_Header
{
   int type 		:2;
   int size		:4;
   int reserved		:4;
   int offbits		:4;
} FILE_Header;

/* Bitmap header */
typedef struct _BMP_Header
{
      int headerSize	:4;
      int width		:4;
      int height	:4;
      int planes 	:2;
      int bitcount	:2;
      int comression	:4;
      int sizeImage	:4;
      int ppmX		:4;
      int ppmY		:4;
      int colUsed	:4;
      int colImportant	:4;

} BMP_Header;


/* Private data structure */
struct _BMP
{
   FILE_Header	fHeader;
   BMP_Header	bmpHeader;
   int*		data;
} BMP;

/* Holds the last error code */
static BMP_STATUS BMP_LAST_ERROR_CODE = 0;


/* Error description strings */
static const char* BMP_ERROR_STRING[] =
{
	"",
	"General error",
	"Could not allocate enough memory to complete the operation",
	"File input/output error",
	"File not found",
	"File is not a supported BMP variant (must be uncompressed 8, 24 or 32 BPP)",
	"File is not a valid BMP image",
	"An argument is invalid or out of range",
	"The requested action is not compatible with the BMP's type"
};


/* Size of the palette data for monochrom bitmaps */
#define BMP_PALETTE_SIZE	0



/*********************************** Forward declarations **********************************/
int		ReadHeader	( BMP* bmp, FILE* f );
int		WriteHeader	( BMP* bmp, FILE* f );


/*********************************** Public methods **********************************/


/**************************************************************
	Creates a blank BMP image with the specified dimensions
	and bit depth.
**************************************************************/
BMP* BMP_Create( int width, int height)
{
   BMP*	bmp;

   if ( height <= 0 || width <= 0 )
   {
      BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
      return NULL;
   }

   if (depth != 1)
   {
	BMP_LAST_ERROR_CODE = BMP_FILE_NOT_SUPPORTED;
	return NULL;
   }


   /* Allocate the bitmap data structure */
   bmp = calloc( 1, sizeof( BMP ) );
   if ( bmp == NULL )
   {
	BMP_LAST_ERROR_CODE = BMP_OUT_OF_MEMORY;
	return NULL;
   }


   /* Set header' default values */
   bmp->fHeader.type 		=19778;
   bmp->fHeader.size		=(height*width+54)/8;
   bmp->fHeader.reserved	=0;
   bmp->fHeader.offbits		=54;
   
   bmp->bmpHeader.headerSize	=40;
   bmp->bmpHeader.width		=width;
   bmp->bmpHeader.height	=height;
   bmp->bmpHeader.planes 	=1;
   bmp->bmpHeader.bitcount	=1;
   bmp->bmpHeader.comression	=0;
   bmp->bmpHeader.sizeImage	=width*height;
   bmp->bmpHeader.ppmX		=0;
   bmp->bmpHeader.ppmY		=0;
   bmp->bmpHeader.colUsed	=0;
   bmp->bmpHeader.colImportant	=0;


	/* Allocate pixels */
	bmp->Data = (UCHAR*) calloc( bmp->Header.size, ((bmp->Header.size)%8)+1 );
	if ( bmp->Data == NULL )
	{
		BMP_LAST_ERROR_CODE = BMP_OUT_OF_MEMORY;
		free( bmp->Palette );
		free( bmp );
		return NULL;
	}


	BMP_LAST_ERROR_CODE = BMP_OK;

	return bmp;
}


/**************************************************************
	Frees all the memory used by the specified BMP image.
**************************************************************/
void BMP_Free( BMP* bmp )
{
	if ( bmp == NULL )
	{
		return;
	}

	if ( bmp->Palette != NULL )
	{
		free( bmp->Palette );
	}

	if ( bmp->Data != NULL )
	{
		free( bmp->Data );
	}

	free( bmp );

	BMP_LAST_ERROR_CODE = BMP_OK;
}


/**************************************************************
	Reads the specified BMP image file.
**************************************************************/
BMP* BMP_ReadFile( const char* filename )
{
	BMP*	bmp;
	FILE*	f;

	if ( filename == NULL )
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
		return NULL;
	}


	/* Allocate */
	bmp = calloc( 1, sizeof( BMP ) );
	if ( bmp == NULL )
	{
		BMP_LAST_ERROR_CODE = BMP_OUT_OF_MEMORY;
		return NULL;
	}


	/* Open file */
	f = fopen( filename, "rb" );
	if ( f == NULL )
	{
		BMP_LAST_ERROR_CODE = BMP_FILE_NOT_FOUND;
		free( bmp );
		return NULL;
	}


	/* Read header */
	if ( ReadHeader( bmp, f ) != BMP_OK || bmp->F_Header.type != 0x4D42 )
	{
		BMP_LAST_ERROR_CODE = BMP_FILE_INVALID;
		fclose( f );
		free( bmp );
		return NULL;
	}


	/* Allocate memory for image data */
	bmp->Data = (UCHAR*) malloc( bmp->BMP_Header.sizeImage );
	if ( bmp->Data == NULL )
	{
		BMP_LAST_ERROR_CODE = BMP_OUT_OF_MEMORY;
		fclose( f );
		free( bmp->Palette );
		free( bmp );
		return NULL;
	}


	/* Read image data */
	if ( fread( bmp->Data, sizeof( UCHAR ), bmp->BMP_Header.sizeImage, f ) != bmp->BMP_Header.sizeImage )
	{
		BMP_LAST_ERROR_CODE = BMP_FILE_INVALID;
		fclose( f );
		free( bmp->Data );
		free( bmp->Palette );
		free( bmp );
		return NULL;
	}


	fclose( f );

	BMP_LAST_ERROR_CODE = BMP_OK;

	return bmp;
}


/**************************************************************
	Writes the BMP image to the specified file.
**************************************************************/
void BMP_WriteFile( BMP* bmp, const char* filename )
{
	FILE*	f;

	if ( filename == NULL )
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
		return;
	}


	/* Open file */
	f = fopen( filename, "wb" );
	if ( f == NULL )
	{
		BMP_LAST_ERROR_CODE = BMP_FILE_NOT_FOUND;
		return;
	}


	/* Write header */
	if ( WriteHeader( bmp, f ) != BMP_OK )
	{
		BMP_LAST_ERROR_CODE = BMP_IO_ERROR;
		fclose( f );
		return;
	}



	/* Write data */
	if ( fwrite( bmp->Data, sizeof( UCHAR ), bmp->BMP_Header.sizeImage, f ) != bmp->BMP_Header.sizeImage )
	{
		BMP_LAST_ERROR_CODE = BMP_IO_ERROR;
		fclose( f );
		return;
	}


	BMP_LAST_ERROR_CODE = BMP_OK;
	fclose( f );
}


/**************************************************************
	Returns the image's width.
**************************************************************/
int BMP_GetWidth( BMP* bmp )
{
	if ( bmp == NULL )
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
		return -1;
	}

	BMP_LAST_ERROR_CODE = BMP_OK;

	return ( bmp->BMP_Header.Width );
}


/**************************************************************
	Returns the image's height.
**************************************************************/
int BMP_GetHeight( BMP* bmp )
{
	if ( bmp == NULL )
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
		return -1;
	}

	BMP_LAST_ERROR_CODE = BMP_OK;

	return ( bmp->BMP_Header.Height );
}


/**************************************************************
	Returns the image's color depth (bits per pixel).
**************************************************************/
int BMP_GetDepth( BMP* bmp )
{
	if ( bmp == NULL )
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
		return -1;
	}

	BMP_LAST_ERROR_CODE = BMP_OK;

	return ( bmp->BMP_Header.BitsPerPixel );
}



/**************************************************************
	Returns the last error code.
**************************************************************/
BMP_STATUS BMP_GetError()
{
	return BMP_LAST_ERROR_CODE;
}


/**************************************************************
	Returns a description of the last error code.
**************************************************************/
const char* BMP_GetErrorDescription()
{
	if ( BMP_LAST_ERROR_CODE > 0 && BMP_LAST_ERROR_CODE < BMP_ERROR_NUM )
	{
		return BMP_ERROR_STRING[ BMP_LAST_ERROR_CODE ];
	}
	else
	{
		return NULL;
	}
}





/*********************************** Private methods **********************************/


/**************************************************************
	Reads the BMP file's header into the data structure.
	Returns BMP_OK on success.
**************************************************************/
int	ReadHeader( BMP* bmp, FILE* f )
{
      char[4] temp4;
      char[2] temp2;
      if ( bmp == NULL || f == NULL ){
	 return BMP_INVALID_ARGUMENT;
      }

	/* The header's fields are read one by one, and converted from the format's
	little endian to the system's native representation. */ 
	fread (&bmp->Header.type, 	1, 2, f);
	fread (&bmp->Header.size, 	1, 2, f);
	fread (&bmp->Header.reserved, 	1, 4, f);
	fread (&bmp->Header.offbits, 	1, 4, f);


	return BMP_OK;
}


/**************************************************************
	Writes the BMP file's header into the data structure.
	Returns BMP_OK on success.
**************************************************************/
int	WriteHeader( BMP* bmp, FILE* f )
{
	if ( bmp == NULL || f == NULL )
	{
		return BMP_INVALID_ARGUMENT;
	}

	/* The header's fields are written one by one, and converted to the format's
	little endian representation. */
	fwrite (&bmp->Header.type, 	1, 2, f);
	fwrite (&bmp->Header.size, 	1, 2, f);
	fwrite (&bmp->Header.reserved, 	1, 4, f);
	fwrite (&bmp->Header.offbits, 	1, 4, f);

	return BMP_OK;
}




