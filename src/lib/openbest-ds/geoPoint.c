#include <stdlib.h>
#include <string.h>

#include "openbest-ds/printOut.h"
#include "openbest-ds/geoPoint.h"
#include "openbest-ds/matrix3.h"
#include "openbest-ds/temperatureGlobals.h"

geoPoint* createGeoPointN()
{
  geoPoint* p= (geoPoint*)malloc(sizeof(geoPoint));
  if ( !p )
    eprintf("malloc failed in createGeoPointN");
  p->latitude= p->longitude= p->elevation= p->x= p->y= p->z= FLT_MAX;
}

geoPoint* createGeoPointC(geoPoint* g)
{
  geoPoint* p= createGeoPointN();
  *p= *g;
  return p;
}

geoPoint* createGeoPoint2(real latitude, real longitude)
{
  geoPoint* p= createGeoPointN();
  p->latitude= latitude;
  p->longitude= longitude;
  
  if ( fabs(latitude) > 90 )
    wprintf("Latitude should be between +/-90");
  
  p->longitude= p->longitude - floor((p->longitude + 180.0)/360.0)*360.0 - 180.0;
  
  computeXYZ_GP(p);
  
  return p;
}

geoPoint* createGeoPoint3(real latitude, real longitude, real elevation)
{
  geoPoint* p= createGeoPoint2(latitude, longitude);
  p->elevation= elevation;
  
  computeXYZ_GP(p);
  
  return p;
}

void destroyGeoPoint(geoPoint* g)
{
  free(g);
}

void computeXYZ_GP(geoPoint* pt)
{
  real R= pt->elevation;
  if ( pt->elevation != FLT_MAX )
  {
    R= earth_radius + pt->elevation/1000;
  }
  
  real phi= pt->longitude * M_PI / 180.0;
  real theta= (-pt->latitude + 90.0) * M_PI / 180.0;
  
  pt->x= R * sin(theta) * cos(phi);
  pt->y= R * sin(theta) * sin(phi);
  pt->z= R * cos(theta);
}

void displayGP(geoPoint* pt)
{
  printf("(GP %f %f %f)", pt->latitude, pt->longitude, pt->elevation);
  fflush(stdout);
}

real distanceGP(geoPoint* pt1, geoPoint* pt2)
{
  real lat1= pt1->latitude * M_PI/180.0;
  real lat2= pt2->latitude * M_PI/180.0;
  
  real long1= pt1->longitude * M_PI/180.0;
  real long2= pt2->longitude * M_PI/180.0;
  
  real dL= long2 - long1;
  
  /* application of the Vincenty formula for spheres: */
  
  real tmp1= cos(lat2)*sin(dL);
  tmp1*= tmp1;
  real tmp2= cos(lat1)*sin(lat2) - sin(lat1)*cos(lat2)*cos(dL);
  tmp2*= tmp2;
  real y= sqrt(tmp1 + tmp2);
  real x= sin(lat1)*sin(lat2) + cos(lat1)*cos(lat2)*cos(dL);
  
  return earth_radius * atan2(y, x);
}

void rotateGP(geoPoint* pt, matrix3 rot)
{
  vector3 i;
  i.x= pt->x; i.y= pt->y; i.z= pt->z;
  vector3 o= mpv(rot, i);
  pt->x= o.x; pt->y= o.y; pt->z= o.z;
  
  real dist= sqrt(pt->x * (pt->x) + (pt->y) * (pt->y) + (pt->z) * (pt->z));
  
  real lat= asin(pt->z/dist)*180.0/M_PI;
  real longitude= atan2(pt->y/dist, pt->x/dist)*180.0/M_PI;
  
  pt->latitude= lat;
  pt->longitude= longitude;
}

geoPoint rotateGPN(geoPoint* pt, matrix3 rot)
{
  vector3 i;
  i.x= pt->x; i.y= pt->y; i.z= pt->z;
  vector3 o= mpv(rot, i);
  pt->x= o.x; pt->y= o.y; pt->z= o.z;
  
  real dist= sqrt(pt->x * (pt->x) + (pt->y) * (pt->y) + (pt->z) * (pt->z));
  
  real lat= asin(pt->z/dist)*180.0/M_PI;
  real longitude= atan2(pt->y/dist, pt->x/dist)*180.0/M_PI;
  
  geoPoint gp;
  
  gp.latitude= lat;
  gp.longitude= longitude;
  gp.elevation= pt->elevation;
  
  return gp;
}

real subsref(geoPoint* pt, const char* S)
{
  if ( strcmp(S, "latitude") == 0 || strcmp(S, "lat") == 0 )
    return pt->latitude;
  else if ( strcmp(S, "longitude") == 0 || strcmp(S, "long") == 0 )
    return pt->longitude;
  else if ( strcmp(S, "elevation") == 0 || strcmp(S, "elev") == 0 || strcmp("S", "height") == 0 )
    return pt->elevation;
  else if ( strcmp(S, "x") == 0 )
    return pt->x;
  else if ( strcmp(S, "y") == 0 )
    return pt->y;
  else if ( strcmp(S, "z") == 0 )
    return pt->z;
  else
    return printf("ERROR: unknown geoPoint property");
}

geoPoint* subsasgn(geoPoint* pt, const char* S, real values)
{
  geoPoint* res= createGeoPointC(pt);
  
  if ( strcmp(S, "latitude") == 0 || strcmp(S, "lat") == 0 )
    res->latitude= values;
  else if ( strcmp(S, "longitude") == 0 || strcmp(S, "long") == 0 )
    res->longitude= values;
  else if ( strcmp(S, "elevation") == 0 || strcmp(S, "elev") == 0 || strcmp("S", "height") == 0 )
    res->elevation= values;
  
  computeXYZ_GP(res);
  return res;
}