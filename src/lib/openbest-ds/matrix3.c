#include "openbest-ds/matrix3.h"
#include <stdlib.h>
#include <math.h>

matrix3 createMatrix3(real a, real b, real c, real d, real e, real f, real g, real h, real i)
{
  matrix3 m;
  setMatrix3(&m, a, b, c, d, e, f, g, h, i);
  return m;
}

void setMatrix3(matrix3* m, real a, real b, real c, real d, real e, real f, real g, real h, real i)
{
  m->t[0]= a;
  m->t[1]= b;
  m->t[2]= c;
  m->t[3]= d;
  m->t[4]= e;
  m->t[5]= f;
  m->t[6]= g;
  m->t[7]= h;
  m->t[8]= i;
}


vector3 vpm(vector3 v, matrix3 m)
{
  vector3 res;
  res.x= v.x*m.t[0] + v.y*m.t[3] + v.z*m.t[6];
  res.y= v.x*m.t[1] + v.y*m.t[4] + v.z*m.t[7];
  res.z= v.x*m.t[2] + v.y*m.t[5] + v.z*m.t[8];
  
  return res;
}

vector3 mpv(matrix3 m, vector3 v)
{
  vector3 res;
  res.x= v.x*m.t[0] + v.y*m.t[1] + v.z*m.t[2];
  res.y= v.x*m.t[3] + v.y*m.t[4] + v.z*m.t[5];
  res.z= v.x*m.t[6] + v.y*m.t[7] + v.z*m.t[8];
  
  return res;
}

matrix3 mpm(matrix3 a, matrix3 b)
{
  matrix3 res;
  res.t[0]= a.t[0]*b.t[0] + a.t[1]*b.t[3] + a.t[2]*b.t[6];
  res.t[1]= a.t[0]*b.t[1] + a.t[1]*b.t[4] + a.t[2]*b.t[7];
  res.t[2]= a.t[0]*b.t[2] + a.t[1]*b.t[5] + a.t[2]*b.t[8];
  
  res.t[0]= a.t[3]*b.t[0] + a.t[4]*b.t[3] + a.t[5]*b.t[6];
  res.t[1]= a.t[3]*b.t[1] + a.t[4]*b.t[4] + a.t[5]*b.t[7];
  res.t[2]= a.t[3]*b.t[2] + a.t[4]*b.t[5] + a.t[5]*b.t[8];
  
  res.t[0]= a.t[6]*b.t[0] + a.t[7]*b.t[3] + a.t[8]*b.t[6];
  res.t[1]= a.t[6]*b.t[1] + a.t[7]*b.t[4] + a.t[8]*b.t[7];
  res.t[2]= a.t[6]*b.t[2] + a.t[7]*b.t[5] + a.t[8]*b.t[8];
  
  return res;
}

real vpv_i(vector3 a, vector3 b)
{
  return a.x*b.x + a.y*b.y + a.z*b.z;
}

matrix3 vpv(vector3 a, vector3 b)
{
  matrix3 res;
  
  res.t[0]= a.x*b.x;
  res.t[1]= a.x*b.y;
  res.t[2]= a.x*b.z;
  res.t[3]= a.y*b.x;
  res.t[4]= a.y*b.y;
  res.t[5]= a.y*b.z;
  res.t[6]= a.z*b.x;
  res.t[7]= a.z*b.y;
  res.t[8]= a.z*b.z;
  
  return res;
}

void displayM(matrix3* m)
{
  printf("%f %f %f\n%f %f %f\n%f %f %f\n", m->t[0], m->t[1], m->t[2], m->t[3], m->t[4], m->t[5], m->t[6], m->t[7], m->t[8]);
}

void displayV(vector3* v)
{
  printf("%f %f %f\n", v->x, v->y, v->z);
}

matrix3 initRotX(real a)
{
  return createMatrix3(1, 0, 0, 0, cos(a), -sin(a), 0, sin(a), cos(a));
}

matrix3 initRotY(real a)
{
  return createMatrix3(cos(a), 0, sin(a), 0, 1, 0, -sin(a), 0, cos(a));
}

matrix3 initRotZ(real a)
{
  return createMatrix3(cos(a), -sin(a), 0, sin(a), cos(a), 0, 0, 0, 1);
}
