#include <stdint.h>
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
#define SLEEPTIME 5

typedef struct _INIT {
	    int count;
	    int bmpsize;
	    int primzsize;
	    uint64_t pixaddr;
	    uint64_t this;
	    uint64_t retaddr;
	} INIT __attribute__((aligned(128)));
	
typedef struct _DIV  {
   unsigned int t;//numerator;
   unsigned int b;//denominator;
} DIV;

typedef struct _POINT {
    unsigned int x;
    unsigned int y;
} POINT;
