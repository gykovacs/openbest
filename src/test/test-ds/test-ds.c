#include <unistd.h>

#include "openbest-ds/basicAlgorithms.h"
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
#include "openbest-ds/lzfx/lzfx.h"
#include "openbest-ds/stationElement2.h"

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

int uniqueTest(int argc, char** argv)
{
    int* t= (int*)malloc(sizeof(int)*10);
    int n= 10;
    int i;

    t[0]= 2;
    t[1]= 1;
    t[2]= 1;
    t[3]= 3;
    t[4]= 2;
    t[5]= 4;
    t[6]= 8;
    t[7]= 0;
    t[8]= 1;
    t[9]= 5;

    uniqueIA(&t, &n);

    for ( i= 0; i < n; ++i )
        printf("%d ", t[i]);
    printf("\n");
    return 0;
}

int modeTest(int argc, char** argv)
{
    real* t= rnalloc(10);
    int n= 10;

    t[0]= 1.2;
    t[1]= -1.4;
    t[2]= 1.8;
    t[3]= -1.4;
    t[4]= 1.75;
    t[5]= 1.8;
    t[6]= 1.8;
    t[7]= 1.8;
    t[8]= 2.0;
    t[9]= 10000;

    printf("%d: %f\n", modeRFA(t, n), modeRMA(t, n));

    free(t);
    return 0;
}

int qsortRATest(int argc, char** argv)
{
    real* t= rnalloc(10);
    int n= 10;

    t[0]= 1.2;
    t[1]= -1.4;
    t[2]= 1.8;
    t[3]= -1.4;
    t[4]= 1.75;
    t[5]= 1.8;
    t[6]= 1.8;
    t[7]= 1.8;
    t[8]= 2.0;
    t[9]= 10000;

    real* tmp= copyRA(t, 10);
    qsortRA(tmp, 10);

    int i;
    for ( i= 0; i < 10; ++i )
        printf("%f ", tmp[i]);

    free(t);
    return 0;
}

int compressTest(int argc, char** argv)
{
    char* testBuffer= (char*)malloc(sizeof(char)*100);
    strcpy(testBuffer,"this is a test string for compression/decompression");
    char* workarea= (char*)malloc(sizeof(char)*strlen(testBuffer));
    unsigned int olen= strlen(testBuffer);

    lzfx_compress((void*)testBuffer, strlen(testBuffer)+1, (void*)workarea, &olen);
    printf("length of compressed string: %d\n", olen);
    printf("compressed data stream:\n");
    int i;
    for ( i= 0; i < olen; ++i )
        printf("%c", workarea[i]);
    printf("\n");

    char* workarea2= (char*)malloc(sizeof(char)*strlen(testBuffer)*2);
    unsigned int olen2= strlen(testBuffer)*2;

    lzfx_decompress((void*)workarea, olen, (void*)workarea2, &olen2);
    printf("%s\n", workarea2);

    free(testBuffer);
    free(workarea);
    free(workarea2);

    return 0;
}

int compressSE2Test(int argc, char** argv)
{
    initDS();

    stationElement2* se= createSE2Test();

    displaySE2(se);

    compressSE2(se);

    decompressSE2(se);

    displaySE2(se);

    destroySE2(se);

    finalizeDS();

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
    bool unique= false;
    bool mode= false;
    bool qsortra= false;
    bool compress= false;
    bool compressSE2b= false;
    int err;
    
    addOption(ot, "--hello", OPTION_BOOL, (char*)&hello, 0, "hello world function to test linking");
    addOption(ot, "--gp", OPTION_BOOL, (char*)&gp, 0, "geoPoint test");
    addOption(ot, "--mv", OPTION_BOOL, (char*)&mv, 0, "vector3 and matrix3 test");
    addOption(ot, "--breakOverlap", OPTION_BOOL, (char*)&breakOverlap, 0, "breakOverlap function test");
    addOption(ot, "--time", OPTION_BOOL, (char*)&time, 0, "time instant test - args: y m d h min s");
    addOption(ot, "--timerange", OPTION_BOOL, (char*)&timer, 0, "time range test");
    addOption(ot, "--pkt", OPTION_BOOL, (char*)&pkt, 0, "primary key table test");
    addOption(ot, "--unique", OPTION_BOOL, (char*)&unique, 0, "unique test");
    addOption(ot, "--mode", OPTION_BOOL, (char*)&mode, 0, "mode function test");
    addOption(ot, "--qsortra", OPTION_BOOL, (char*)&qsortra, 0, "qsortra function test");
    addOption(ot, "--compress", OPTION_BOOL, (char*)&compress, 0, "compression/decompression test");
    addOption(ot, "--compressSE2", OPTION_BOOL, (char*)&compressSE2b, 0, "compress/decompress StationElement2");
    
    if ( processArgs(ot, &argc, argv) )
    {
        destroyOptionTable(ot);
        return -1;
    }
    
    if ( hello )
      err= helloTest(argc, argv);
    else if ( gp )
      err= gpTest(argc, argv);
    else if ( mv )
      err= mvTest(argc, argv);
    else if ( breakOverlap )
      err= breakOverlapTest(argc, argv);
    else if ( time )
      err= timeInstantTest(argc, argv);
    else if ( timer )
      err= timeRangeTest(argc, argv);
    else if ( pkt )
      err= primaryKeyTableTest(argc, argv);
    else if ( unique )
      err= uniqueTest(argc, argv);
    else if ( mode )
      err= modeTest(argc, argv);
    else if ( qsortra )
      err= qsortRATest(argc, argv);
    else if ( compress )
      err= compressTest(argc, argv);
    else if ( compressSE2b )
      err= compressSE2Test(argc, argv);

    destroyOptionTable(ot);
    
    return err;
}
