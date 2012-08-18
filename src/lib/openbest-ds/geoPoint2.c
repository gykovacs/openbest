#include "openbest-ds/geoPoint2.h"
#include <stdlib.h>

geoPoint2* createGeoPoint2N()
{
  geoPoint2* p= (geoPoint2*)malloc(sizeof(geoPoint2));
  if ( !p )
    eprintf("malloc failed in createGeoPointN");
  p->lat_uncertainty= p->long_uncertainty= p->elev_uncertainty= p->latitude= p->longitude= p->elevation= p->x= p->y= p->z= FLT_MAX;
}

geoPoint2* createGeoPoint2C(geoPoint2* g)
{
  geoPoint2* p= createGeoPoint2N();
  *p= *g;
  return p;
}

geoPoint2* createGeoPoint21(geoPoint* gp)
{
  geoPoint2* p= createGeoPoint23(gp->latitude, gp->longitude, gp->elevation);
  
  computeXYZ_GP2(p);
  return p;
}

geoPoint2* createGeoPoint22(real latitude, real longitude)
{
  geoPoint2* p= createGeoPoint2N();
  p->latitude= latitude;
  p->longitude= longitude;
  p->elevation= 0;
  
  computeXYZ_GP2(p);
  
  return p;
}

geoPoint2* createGeoPoint23(real latitude, real longitude, real elevation)
{
  geoPoint2* p= createGeoPoint22(latitude, longitude);
  p->elevation= elevation;
  
  computeXYZ_GP2(p);
  
  return p;
}

geoPoint2* createGeoPoint26(real latitude, real longitude, real elevation, real lat_uncertainty, real long_uncertainty, real elev_uncertainty)
{
  geoPoint2* p= createGeoPoint23(latitude, longitude, elevation);
  p->lat_uncertainty= lat_uncertainty;
  p->long_uncertainty= long_uncertainty;
  p->elev_uncertainty= elev_uncertainty;
  
  return p;
}

void computeXYZ_GP2(geoPoint2* pt)
{

  if ( pt->elevation < 0 )
      pt->elevation= 0;
  real R= pt->elevation;
  if ( pt->elevation != FLT_MAX && pt->elevation != -FLT_MAX && pt->elevation >= -1000 && pt->elevation <= 10000)
  {
    R= earth_radius + pt->elevation/1000;
  }
  
  real phi= pt->longitude * M_PI / 180.0;
  real theta= (-pt->latitude + 90.0) * M_PI / 180.0;
  
  pt->x= R * sin(theta) * cos(phi);
  pt->y= R * sin(theta) * sin(phi);
  pt->z= R * cos(theta);
  //displayGP2(pt);
}

void displayGP2(geoPoint2* pt)
{
  printf("(GP2 %f %f %f)", pt->latitude, pt->longitude, pt->elevation);
  fflush(stdout);
}

real distanceGP2(geoPoint2* pt1, geoPoint2* pt2)
{
  real lat1= pt1->latitude * M_PI/180.0;
  real lat2= pt2->latitude * M_PI/180.0;
  
  real long1= pt1->longitude * M_PI/180.0;
  real long2= pt2->longitude * M_PI/180.0;
  
  real dL= long2 - long1;
  
  /* application of the Vincenty formula for spheres: */
  
  real A= cos(lat2);
  real B= sin(lat2);
  real C= cos(lat1);
  real D= sin(lat1);
  real E= A*cos(dL);
  real tmp= A*sin(dL);
  real tmp2= C*B - D*E;
  real Y= sqrt(tmp*tmp + tmp2*tmp2);
  real X= D*B + C*E;
  
  return earth_radius * atan2(Y, X);
}

int isnanGP2(geoPoint2* gp)
{
  if ( gp->latitude == FLT_MAX || gp->longitude == FLT_MAX )
    return 0;
  return 1;
}

void rotateGP2(geoPoint2* pt, matrix3 rot)
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

geoPoint2* setUncertaintyGP2(geoPoint2* gp, real lat_error, real long_error, real elev_error)
{
  gp->lat_uncertainty= lat_error;
  gp->long_uncertainty= long_error;
  gp->elev_uncertainty= elev_error;
  
  return gp;
}

void setGeoPoint2(geoPoint2* gp, real latitude, real longitude, real elevation, real lat_uncertainty, real long_uncertainty, real elev_uncertainty)
{
  gp->latitude= latitude;
  gp->longitude= longitude;
  gp->elevation= elevation;
  gp->lat_uncertainty= lat_uncertainty;
  gp->long_uncertainty= long_uncertainty;
  gp->elev_uncertainty= elev_uncertainty;
  computeXYZ_GP2(gp);
}

bool isValidGP2(geoPoint2* gp)
{
    if ( gp->latitude <= -999 || gp->longitude <= -999 || gp->latitude >= 999 || gp->longitude >= 999 )
        return false;
    return true;
}
