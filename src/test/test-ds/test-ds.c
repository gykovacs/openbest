#include <unistd.h>

#include "openbest-ds/hello-openbest.h"
#include "openbest-ds/geoPoint.h"
#include "openbest-ds/geoPoint2.h"
#include "openbest-ds/geoPoint2Vector.h"
#include "openbest-ds/matrix3.h"
#include "openbest-ds/timeInstant.h"
#include "openbest-ds/timeRange.h"
#include "openbest-ds/primaryKeyTables.h"
#include "openbest-ds/init-ds.h"
#include "openbest-io/loadCountryNames.h"
#include "openbest-io/loadStationSourceTypes.h"

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

int timeInstantTest(int argc, char** argv)
{
  timeInstant ti;
  ti.year= atoi(argv[1]);
  ti.month= atoi(argv[2]);
  ti.day= atoi(argv[3]);
  ti.hour= atoi(argv[4]);
  ti.minute= atoi(argv[5]);
  ti.second= atoi(argv[6]);
  
  real t= datenum(&ti);
  printf("%f\n", t);
  
  timeInstant ti2;
  convertFromSerialTime(t, &ti2);
  displayTI(&ti2);
  
  return 0;
}

int timeRangeTest(int argc, char** argv)
{
  timeInstant t1= createTimeInstantN(1984, 5, 9, 2, 10, 17);
  timeInstant t2= createTimeInstantN(2012, 7, 5, 13, 4, 58);
  
  displayTI(&t1);
  newline();
  displayTI(&t2);
  newline();
  
  timeRange tr= createTimeRangeN2a(&t1, &t2);
  displayTR(&tr);
  newline();
  
  return 0;
}

int primaryKeyTableTest(int argc, char** argv)
{
  primaryKeyTable pkt= createPrimaryKeyTableN();
  
  insertIntoPKT(&pkt, "apple", 10);
  insertIntoPKT(&pkt, "appletree", 10);
  insertIntoPKT(&pkt, "grape", 10);
  insertIntoPKT(&pkt, "cherry", 11);
  insertIntoPKT(&pkt, "pineapple", 12);
  insertIntoPKT(&pkt, "banana", 14);
  
  printf("%d\n", lookupValuePKT(&pkt, "apple"));
  printf("%d\n", lookupValuePKT(&pkt, "grape"));
  printf("%d\n", lookupValuePKT(&pkt, "cherry"));
  
  char** tmp;
  tmp= lookupKeysPKT(&pkt, 10);
  int i= 0;
  while ( tmp[i] != NULL )
    printf("%s\n", tmp[i++]);
  
  displayPKTbyValue(&pkt);
  printf("----------------\n");
  displayPKTbyKeys(&pkt);
  
  destroyPKT(&pkt);
  
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
    bool time= false;
    bool timer= false;
    bool pkt= false;
    
    addOption(ot, "--hello", OPTION_BOOL, (char*)&hello, 0, "hello world function to test linking");
    addOption(ot, "--gp", OPTION_BOOL, (char*)&gp, 0, "geoPoint test");
    addOption(ot, "--mv", OPTION_BOOL, (char*)&mv, 0, "vector3 and matrix3 test");
    addOption(ot, "--breakOverlap", OPTION_BOOL, (char*)&breakOverlap, 0, "breakOverlap function test");
    addOption(ot, "--time", OPTION_BOOL, (char*)&time, 0, "time instant test - args: y m d h min s");
    addOption(ot, "--timerange", OPTION_BOOL, (char*)&timer, 0, "time range test");
    addOption(ot, "--pkt", OPTION_BOOL, (char*)&pkt, 0, "primary key table test");
    
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
    else if ( time )
      return timeInstantTest(argc, argv);
    else if ( timer )
      return timeRangeTest(argc, argv);
    else if ( pkt )
      return primaryKeyTableTest(argc, argv);
    
    return 0;
}
