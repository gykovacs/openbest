#include <openbest-ds/hello-openbest.h>
#include <openbest-ds/geoPoint.h>

/**
* This simple test application calls a test function from the openbest-ds (data structures) shared object file.
*/
int main(int argc, char** argv)
{
    helloOpenBEST();
    
    geoPoint* gp= createGeoPoint3(10, 12, 14);
    display(gp);
    
    return 0;
}