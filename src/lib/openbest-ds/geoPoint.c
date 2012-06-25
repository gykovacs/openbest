#include <stdlib.h>

#include <openbest-ds/geoPoint.h>

geoPoint* createGeoPointN()
{
  geoPoint* p= (geoPoint*)malloc(sizeof(geoPoint));
  p->latitude= p->longitude= p->elevation= p->x= p->y= p->z= FLT_MAX;
}

geoPoint* createGeoPointC(geoPoint* g)
{
  geoPoint* p= createGeoPointN();
  *p= *g;
  return p;
}

geoPoint* createGeoPoint2(float latitude, float longitude)
{
  geoPoint* p= createGeoPointN();
  p->latitude= latitude;
  p->longitude= longitude;
  
  if ( fabs(latitude) > 90 )
    printf("Latitude should be between +/-90");
  
  p->longitude= p->longitude - floor((p->longitude + 180.0)/360.0)*360.0 - 180.0;
  
  computeXYZ(p);
  
  return p;
}

geoPoint* createGeoPoint3(float latitude, float longitude, float elevation)
{
  geoPoint* p= createGeoPoint2(latitude, longitude);
  p->elevation= elevation;
  
  computeXYZ(p);
  
  return p;
}

void computeXYZ(geoPoint* pt)
{
  float R= pt->elevation;
  if ( pt->elevation != FLT_MAX )
  {
    R= earth_radius + pt->elevation/1000;
  }
  
  float phi= pt->longitude * M_PI / 180.0;
  float theta= (-pt->latitude + 90.0) * M_PI / 180.0;
  
  pt->x= R * sin(theta) * cos(phi);
  pt->y= R * sin(theta) * sin(phi);
  pt->z= R * cos(theta);
}

void display(geoPoint* pt)
{
  printf("(%f %f %f)", pt->latitude, pt->longitude, pt->elevation);
}

float distance(geoPoint* pt1, geoPoint* pt2)
{
  float lat1= pt1->latitude * M_PI/180.0;
  float lat2= pt2->latitude * M_PI/180.0;
  
  float long1= pt1->longitude * M_PI/180.0;
  float long2= pt2->longitude * M_PI/180.0;
  
  float dL= long2 - long1;
  
  /* application of the Vincenty formula for spheres: */
  
  float tmp1= cos(lat2)*sin(dL);
  tmp1*= tmp1;
  float tmp2= cos(lat1)*sin(lat2) - sin(lat1)*cos(lat2)*cos(dL);
  tmp2*= tmp2;
  float y= sqrt(tmp1 + tmp2);
  float x= sin(lat1)*sin(lat2) + cos(lat1)*cos(lat2)*cos(dL);
  
  return earth_radius * atan2(y, x);
}
