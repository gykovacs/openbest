#include "openbest-ds/tile.h"
#include <stdlib.h>
#include <math.h>

tile createTile(real a, real b, real c, real d)
{
  tile t;
  t.xmin= a;
  t.ymin= b;
  t.xmax= c;
  t.ymax= d;
  
  return t;
}

tile* createTileP(real a, real b, real c, real d)
{
  tile* p= (tile*)malloc(sizeof(tile));
  
  p->xmin= a;
  p->ymin= b;
  p->xmax= c;
  p->ymax= d;
  
  return p;
}

tile* createTileC(tile* p)
{
  return createTileP(p->xmin, p->ymin, p->xmax, p->ymax);
}

void destroyTile(tile* p)
{
  free(p);
}

void setTile(tile* p, real a, real b, real c, real d)
{
  p->xmin= a;
  p->ymin= b;
  p->xmax= c;
  p->ymax= d;
}
