#ifndef _VECTOR3_H_
#define _VECTOR3_H_

typedef struct
{
  float x;
  float y;
  float z;
} vector3;

vector3 createVector3(float x, float y, float z);

void setVector3(vector3* v, float x, float y, float z);

#endif