#include <stdio.h>
#include <libspe.h>
#include <stdlib.h>
#include <time.h>

extern spe_program_handle_t primer_spu;
extern spe_program_handle_t point_spu;

#define SPES 6
speid_t spid[SPES];
//TODO: DEFINE STRUCTS addr[SPES];


int main() {
  int i;
  int j;
  
  int gridsize=400;
  
  spid[i]= spe_create_thread(0, &point_spu, &addr, NULL, -1, 0);
  
  
  //wait for everybody to finish
  for(i =0; i<SPES; i++) {
    spe_wait(spid[i],NULL,0);
  }
  return 0;
}

