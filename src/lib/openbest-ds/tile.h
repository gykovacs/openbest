#ifndef _TILE_H_
#define _TILE_H_

#include "openbest-ds/config.h"

typedef struct
{
  real xmin;
  real ymin;
  real xmax;
  real ymax;
}tile;

tile createTile(real a, real b, real c, real d);

tile* createTileP(real a, real b, real c, real d);

tile* createTileC(tile* p);

void destroyTile(tile* p);

void setTile(tile* p, real a, real b, real c, real d);

#endif