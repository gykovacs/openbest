#include "openbest-ds/vector3.h"

vector3 createVector3(real x, real y, real z)
{
  vector3 v;
  setVector3(&v, x, y, z);
  return v;
}

void setVector3(vector3* v, real x, real y, real z)
{
  v->x= x;
  v->y= y;
  v->z= z;
}
