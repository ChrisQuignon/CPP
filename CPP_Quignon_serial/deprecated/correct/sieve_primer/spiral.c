#include "ds.h"

//defines the POINT on the Framebuffer of the n th number
//TODO: implement
POINT spiral(long int n){
  POINT ret;
  long int dir, count, dist;
  int circle;
  
  ret.x=info.xres/2;
  ret.y=info.yres/2;
  
  //last corner before n
  for(count = 0; count+ dist < n; count = count+dist){
    dist = dist + count%2;
    dir=(dir+1)%4;
    
    //
    switch dir {
      case 0:
	ret.x=ret.x+dist;
	break;
      case 1:
	ret.y=ret.y+dist;
	break;
      case 2:
	ret.x=ret.x-dist;
	break;
      case 3:
	ret.y=ret.y-dist;
	break;
      default:
	break;
    }
  }
  //rest to reach n
  dist=n-count;
  if(dist>0){
      switch dir {
      case 0:
	ret.x=ret.x+dist;
	break;
      case 1:
	ret.y=ret.y+dist;
	break;
      case 2:
	ret.x=ret.x-dist;
	break;
      case 3:
	ret.y=ret.y-dist;
	break;
      default:
	break;
      }
  }
  return ret;  
}