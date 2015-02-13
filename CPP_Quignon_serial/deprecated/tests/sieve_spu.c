#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <asm/ps3fb.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <libspe.h>
#include "ds.h"

SIEVE addr;
struct ps3fb_ioctl_res info;
struct fb_vblank vblank;
int fd, disp_size, margin_size, fb_size;

void spiral(long int n){
  POINT ret;
  long int dir, gone, dist, i;
  int circle;
  
  dist=0;
  dir=0;
  gone=0;
  
  ret.x=info.xres/2;
  ret.y=info.yres/2;
  
  //last corner before n
  for(gone = 0; dist<(n-gone); gone++){
    
    //increase square size every second iteration
    if((dir%2)==1){dist++;}
        
    printf("dist = %i\n", dist);
    printf("dir = %i\n", dir);
    
    //
    switch (dir) {
      case 1:
	for(i=0; i<dist; i++){
	  ret.x++;
	  gone++;
	  if(isprime(gone)) drawpoint(ret, white);
	}
	break;
      case 2:
	for(i=0; i<dist; i++){
	  ret.y++;
	  gone++;
	  if(isprime(gone)) drawpoint(ret, white);
	}
	break;
      case 3:
	for(i=0; i<dist; i++){
	  ret.x--;
	  gone++;
	  if(isprime(gone)) drawpoint(ret, white);
	}
	break;
      case 0:
	for(i=0; i<dist; i++){
	  ret.y--;
	  gone++;
	  if(isprime(gone)) drawpoint(ret, white);
	}
	break;
      default:
	break;
    }
    //drawpoint(ret, white);
    printf("x = %i - y = %i\n", ret.x, ret.y);
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

void drawpoint(POINT p, COLOR c) {
  
  //int x = 128;
  //int y = 128;
  //int x_size, y_size;

  unsigned int *pix_addr;
  unsigned int frame = 0;
  
  /* Number of rows to be drawn */
  //y_size = info.yres - 2 * info.yoff;
  
  /* Number of columns to be drawn */
  //x_size = info.xres - 2 * info.xoff;
  
  /* Wait for vertical sync signal */
  //ioctl(fd, FBIO_WAITFORVSYNC, 0);
	
  // Set pixel Adress MODIFIED
  pix_addr = (unsigned int*)
	(addr + p.y * info.xres * BYTES_PER_PIXEL);
  pix_addr += p.x;
  *pix_addr = (c.r << 16  | c.g << 8 | c.b);

  /* Identify buffer for the next flip */
  ioctl(fd, PS3FB_IOCTL_FSEL, (unsigned long)&frame);
  
}

int main(unsigned long long speid,
	 unsigned long long argp,
	 unsigned long long envp
	 ) {
  int i;
  POINT p;
  COLOR white, black;
    
  white.r = 0xff;
  white.g = 0xff;
  white.b = 0xff;
    
  black.r = 0x00;
  black.g = 0x00;
  black.b = 0x00;
  //get data
  mfc_get(&addr, argp, sizeof(SIEVE), 0, 0, 0);
  mfc_write_tag_mask(1 << 0);
  mfc_read_tag_status_all();
  
  //"randomize"
    /* Open the framebuffer device file */
  if ((fd = open("/dev/fb0", O_RDWR)) < 0) {
      fprintf(stderr, "error open:%d\n", fd);
      return -1;
  }

  /* Make sure the vsync signal is accessible */
  ioctl(fd, FBIOGET_VBLANK, (unsigned long)&vblank);
  if (!(vblank.flags & FB_VBLANK_HAVE_VSYNC)) {
      fprintf(stderr, "Error accessing vertical sync\n");
      close(fd);
      return -1;
  }
  
  /* Acquire information about display/framebuffer */
  if (ioctl(fd, PS3FB_IOCTL_SCREENINFO, 
	(unsigned long)&info) < 0) {
      fprintf(stderr, "Error accessing screen info\n");
      close(fd);
      return -1;
  }
  
  /* Determine buffer size and memory map buffer to addr */
  disp_size = info.xres * info.yres * BYTES_PER_PIXEL;
  margin_size = info.xres * info.yoff * 2 * BYTES_PER_PIXEL;
  fb_size = disp_size - margin_size;
  addr = mmap(NULL, fb_size, PROT_WRITE, MAP_SHARED, fd, 0);
  if (addr == MAP_FAILED) {
      fprintf(stderr, "Error mapping framebuffer to memory\n");
      close(fd);
      return -1;
  }
  

  ioctl(fd, PS3FB_IOCTL_ON, 0);
  memset(addr, 0, fb_size);
  
  //STUFF GOES IN HERE12
  
  
  ioctl(fd, PS3FB_IOCTL_OFF, 0);
  munmap(NULL, fb_size);
  close(fd);
  return 0;
}


