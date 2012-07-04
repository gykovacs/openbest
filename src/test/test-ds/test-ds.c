#include <unistd.h>

#include "openbest-ds/config.h"

#include "openbest-ds/hello-openbest.h"
#include "openbest-ds/geoPoint.h"
#include "openbest-ds/geoPoint2.h"
#include "openbest-ds/geoPoint2Vector.h"
#include "openbest-ds/matrix3.h"

#include "openbest-ds/optionTable.h"

int helloTest(int argc, char** argv)
{
  helloOpenBEST();
  return 0;
}

int gpTest(int argc, char** argv)
{
  geoPoint* gp= createGeoPoint3(10, 12, 14);
  displayGP(gp);
  return 0;
}

int mvTest(int argc, char** argv)
{
  matrix3 m= createMatrix3(1, 2, 1, 3, 1, 2, 1, 2, 1);
  vector3 v= createVector3(1, 1, 2);
  
  displayM(&m);
  displayV(&v);
  
  vector3 r= mpv(m,v);
  displayV(&r);
  
  return 0;
}

int breakOverlapTest(int argc, char** argv)
{
  real tile1[4]= {0,-1,1,0};
  real tile2[4]= {-2, -3, 5, 6};
  real* region1;
  real* region2;
  real* intersect;
  int cp1;
  int cp2;
  int cp3;
  int i;
  
  breakOverlap((real*)tile1, (real*)tile2, &region1, &region2, &intersect, &cp1, &cp2, &cp3);
  
  printf("cp1: %d\n", cp1);
  printf("cp2: %d\n", cp2);
  printf("cp3: %d\n", cp3);
  
  for ( i= 0; i < cp1; ++i )
    printf("%f %f %f %f\n", region1[i*4 + 0], region1[i*4 + 1], region1[i*4 + 2], region1[i*4 + 3]);
  printf("\n");
  for ( i= 0; i < cp2; ++i )
    printf("%f %f %f %f\n", region2[i*4 + 0], region2[i*4 + 1], region2[i*4 + 2], region1[2*4 + 3]);
  printf("\n");
  for ( i= 0; i < cp3; ++i )
    printf("%f %f %f %f\n", intersect[i*4 + 0], intersect[i*4 + 1], intersect[i*4 + 2], intersect[2*4 + 3]);
  printf("\n");
  
  return 0;
}

/**
* This simple test application calls a test function from the openbest-ds (data structures) shared object file.
*/
int main(int argc, char** argv)
{
    optionTable* ot= createOptionTable();
    
    setPrefix(ot, "openbest data structure tests");
    addUsage(ot, "test-ds <option-name> arg1 arg2 ...");
    
    bool hello= false;
    bool gp= false;
    bool mv= false;
    bool breakOverlap= false;
    
    addOption(ot, "--hello", OPTION_BOOL, (char*)&hello, 0, "hello world function to test linking");
    addOption(ot, "--gp", OPTION_BOOL, (char*)&gp, 0, "geoPoint test");
    addOption(ot, "--mv", OPTION_BOOL, (char*)&mv, 0, "vector3 and matrix3 test");
    addOption(ot, "--breakOverlap", OPTION_BOOL, (char*)&breakOverlap, 0, "breakOverlap function test");
    
    if ( processArgs(ot, &argc, argv) )
      return -1;
    
    if ( hello )
      return helloTest(argc, argv);
    else if ( gp )
      return gpTest(argc, argv);
    else if ( mv )
      return mvTest(argc, argv);
    else if ( breakOverlap )
      return breakOverlapTest(argc, argv);
    
    return 0;
}