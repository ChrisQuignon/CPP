#include <stdio.h>
#include <libspe.h>
#include <stdlib.h>
#include <gmp.h>
#include "bmpfile.h"
#include "bmpfile.c"
#include "defs.c"

//macros for minimum and maximum
#define MIN(a,b) ((a) > (b) ? (b) : (a))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

#define SPES 6

// The pixcount has to be a multiple of 36
// the used alignment algorithm have 4 and 9 pixles as in a minimal traversion
// the algorithm may be stopped when both algorithm have finished a traversion
// this is the case for every multiple for every of their least common multiple which is 36
#define PIXCOUNT 36

//three handles - one for every alignment algorithm
extern spe_program_handle_t spiral_spu;
extern spe_program_handle_t ncurve_spu;
extern spe_program_handle_t hilbert_spu;

/*TODO:
one more handle to compute the probabiblity of one array of pixels
*/

speid_t spid[SPES];

//every alignment algorithm gets its own initiation data structure
INIT hilbert __attribute__((aligned(128)));
INIT spiral __attribute__((aligned(128)));
INIT ncurve __attribute__((aligned(128)));

// every algorithm gets its own 2 dimensional pixelarray
// where the return values get stored
rgb_pixel_t **hilbertpxs __attribute__((aligned(128)));
rgb_pixel_t **spiralpxs __attribute__((aligned(128)));
rgb_pixel_t **ncurvepxs __attribute__((aligned(128)));

// every alignment algorithm gets its own bmpfile in which it can be outputed
bmpfile_t *hilbertbmp;
bmpfile_t *ncurvebmp;
bmpfile_t *spiralbmp;

// the initial primes get stored in their own 2 dimensional array
// from which all alignment algorithms read
rgb_pixel_t ** pixels __attribute__((aligned(128)));


int main( int argc, char* argv[] ){
  
  int i, j, k; 		// iteration Variables
  int its, ith, itn; 	// iteration values for the alignment algorithms
  int last;		// last
  mpz_t posi;		// mpz to store current value to be checked for prime
  char file[256];	// string for the output files
  DIV d;		// 
  
  
   
  /* Check arguments */
  if (argc < 4) {
    printf("Usage: %s filename length start.\n", argv[0]);
    return 1;
  } 
    
  mpz_init(posi);			//initiation of the mpz
  mpz_set_str (posi, argv[3], 10);	//read invocation Values as mpz_t to basis 10
  
  //printf("length = %i\n", atoi(argv[2]));
  
  // every alignement algorithm has its own iteration value next to the wished bmp size
  // this value determines the bmp size of this alignment algorithm
  ith = (int) log((double) (atoi(argv[2])))/log(2); //log to base 2
  ith = pow(2, ith);
  //printf("ith = %i\n", ith);

  its = atoi(argv[2])/2;
  its = its*2+1;
  //printf("its = %i\n", its);
  
  itn = (int) log((double) (atoi(argv[2])))/log(3); //log to base 3
  itn = pow(3, itn);
  //printf("itn = %i\n", itn);
  
  // last value for which we have to determine if it is a primes
  // is the square of the maximum of the algorithm bmp size
  last=MAX(MAX(itn, its), ith);
  last=last*last;
  
  // the BMP files for the algorithm:
  // their creation is quite time intensive but has to be done here
  hilbertbmp=bmp_create(hilbert.bmpsize, hilbert.bmpsize, BMPBITS);
  ncurvebmp=bmp_create(ncurve.bmpsize, ncurve.bmpsize, BMPBITS);
  spiralbmp=bmp_create(spiral.bmpsize, spiral.bmpsize, BMPBITS);
  
  // the 2 dimensional array that stores the primes ha s length of PIXCOUNT
  // and it hold as many primes as needed
  pixels=calloc(last/PIXCOUNT+1, sizeof(rgb_pixel_t *));
  for(i=0; i<last/PIXCOUNT+1; i++){
    pixels[i]=malloc(PIXCOUNT*sizeof(rgb_pixel_t));}
  
  /*HILBERT INVOCATION:*/
  
  // the 2 dimensional for the return values has to be set up before invacation
  // only then their pointers are set - it is not important to have null set arrays
  hilbertpxs=malloc(ith, sizeof(rgb_pixel_t *));
  for(i=0; i<ith; i++){
    hilbertpxs[i]=calloc(ith, sizeof(rgb_pixel_t));}
  
  //Set all values for initiation
  hilbert.count=0;
  hilbert.bmpsize=ith;
  hilbert.primzsize=PIXCOUNT;
  hilbert.pixaddr= &pixels;
  hilbert.this= &hilbert;
  hilbert.retaddr= hilbertpxs;

  /*printout for testing:
  printf("\nHILBERT:\n");
  printf("count = %i \n", hilbert.count);
  printf("bmpsize = %i \n", hilbert.bmpsize);
  printf("primzsize = %i \n", hilbert.primzsize);
  printf("pixaddr = %i \n", (int)hilbert.pixaddr);
  printf("this = %i \n", (int)hilbert.this);
  printf("retaddr = %i \n", (int)hilbert.retaddr);*/

  spid[0]= spe_create_thread(0, &hilbert_spu, &hilbert, NULL, -1, 0);

  /*TODO: SPIRAL INVOCATION*/
  
  /*TODO: NCURVE INVOCATION*/

  
  // now all prime are computed and stored in the pixels array   
  //SHOULD BE ATOMAR!
  for (i=0; i<=last/PIXCOUNT; i++){
    /*create pixels*/
    for(j=0; j<PIXCOUNT; j++){
      if(mpz_probab_prime_p(posi, PRIMEITS)) k = 255; //check if current position is prime}
      else k = 0;
      // set pixel
      pixels[i][j].blue=k;
      pixels[i][j].green=k;
      pixels[i][j].red=k;
      pixels[i][j].alpha=k;
      mpz_add_ui (posi, posi, 1);
    }
    //increase the value of the INITS, so the array can be retrieved by the SPEs
    hilbert.count++;
    //ncurve.count++;
    //spiral.count++;
  }

  //wait for everybody to finish
  for(i =0; i<1; i++) {
    spe_wait(spid[i],NULL,0);}
    
    
  // the pictures are now at their 2 dimensional array
  
  /*TODO: deg0 - 135
  * now every bmp has to be traversed at all multiples of 45°
  * these arrays have to be sent to the SPUs to determine their probability
  * the SPUs will send back these probabilities which then can be output
  */
  
  // the aligned pixel array are now insertet to their bmps:
  hilbertbmp->pixels=hilbertpxs;
  //ncurvebmp->pixels=ncurvepxs;
  //spiralbmp->pixels=spiralpxs;

  
  /*save bmp
  strcpy(file, argv[1]);
  strcat(file, "_HILBERT.bmp");
  //the saving is quite time intensive
  bmp_save(hilbertbmp, file);*/
  
  /*TODO: save other alignement algorithms*/
  
  return 0;
}

