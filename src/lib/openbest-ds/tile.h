#ifndef _TILE_H_
#define _TILE_H_

#include "openbest-ds/config-ds.h"

/** tile structure*/
typedef struct
{
  real xmin;
  real ymin;
  real xmax;
  real ymax;
}tile;

/** creates a new tile
  * @param a xmin
  * @param b ymin
  * @param c xmax
  * @param d ymax
  * @returns new tile by value
  */
tile createTile(real a, real b, real c, real d);

/** creates a new tile and returns its pointer
  * @param a xmin
  * @param b ymin
  * @param c xmax
  * @param d ymax
  * @returns new tile by pointer
  */
tile* createTileP(real a, real b, real c, real d);

/** copies the parameter tile and returns its pointer
  * @param a xmin
  * @param b ymin
  * @param c xmax
  * @param d ymax
  * @param new tile by pointer
  */
tile* createTileC(tile* p);

/**
  * destroys the parameter tile
  * @param p tile to destroy
  */
void destroyTile(tile* p);

/**
  * sets the members of the tile
  * @param p tile to set the parameters of
  * @param a xmin
  * @param b ymin
  * @param c xmax
  * @param d ymax
  */
void setTile(tile* p, real a, real b, real c, real d);

#endif
