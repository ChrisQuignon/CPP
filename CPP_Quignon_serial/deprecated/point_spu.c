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


#define BYTES_PER_PIXEL 4


void drawpoint(int fd, void *fb_addr, struct ps3fb_ioctl_res *info, POINT p, COLOR c) {
  
  //int x = 128;
  //int y = 128;
  //int x_size, y_size;

  unsigned int *pix_addr;
  unsigned int frame = 0;
  
  /* Number of rows to be drawn */
  //y_size = info->yres - 2 * info->yoff;
  
  /* Number of columns to be drawn */
  //x_size = info->xres - 2 * info->xoff;
  
  /* Wait for vertical sync signal */
  //ioctl(fd, FBIO_WAITFORVSYNC, 0);
	
  // Set pixel Adress MODIFIED
  pix_addr = (unsigned int*)
	(fb_addr + p.y * info->xres * BYTES_PER_PIXEL);
  pix_addr += p.x;
  *pix_addr = (c.r << 16  | c.g << 8 | c.b);

  /* Identify buffer for the next flip */
  ioctl(fd, PS3FB_IOCTL_FSEL, (unsigned long)&frame);
  
}

void drawline(int fd, void *fb_addr, struct ps3fb_ioctl_res *info, POINT p0, POINT p1, COLOR c){
  
  //http://www.fact-index.com/b/br/bresenham_s_line_algorithm_c_code.html
  int i;
  int steep = 1;
  int sx, sy;  // step positive or negative (1 or -1) 
  int dx, dy;  // delta (difference in X and Y between points) 
  int e;
  
  // optimize for vertical and horizontal lines here 
  dx = abs(p1.x - p0.x);
  sx = ((p1.x - p0.x) > 0) ? 1 : -1;
  dy = abs(p1.y - p0.y);
  sy = ((p1.y - p0.y) > 0) ? 1 : -1;
  
  e = (dy << 1) - dx;
  for (i = 0; i < dx; i++) {
    if (steep) { drawpoint(fd, fb_addr, info, p0,c);}
    else { drawpoint(fd, fb_addr, info, p0, c);}
    while (e >= 0) {
      p0.y += sy;
      e -= (dx << 1);}
    p0.x += sx;
    e += (dy << 1);} 
}


int main() {

  void *addr;
  struct ps3fb_ioctl_res info;
  struct fb_vblank vblank;
  int fd, disp_size, margin_size, fb_size, i;
  
  green.r = 0x00;
  green.g = 0xff;
  green.b = 0x00;
  
  white.r = 0xff;
  white.g = 0xff;
  white.b = 0xff;
  
  black.r = 0x00;
  black.g = 0x00;
  black.b = 0x00;
  
  p1.x = 120;
  p1.y = 120;
  p1.z = 120;
  
  p2.x = 150;
  p2.y = 250;
  p2.z = 50; 
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
    
    drawpoint(fd, addr, &info, p1, white);
    
    ioctl(fd, PS3FB_IOCTL_OFF, 0);
    munmap(NULL, fb_size);
    close(fd);
  
  return 0;
}