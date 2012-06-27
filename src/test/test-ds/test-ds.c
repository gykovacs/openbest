#include <openbest-ds/hello-openbest.h>
#include <openbest-ds/geoPoint.h>

#include <openbest-ds/matrix3.h>

/**
* This simple test application calls a test function from the openbest-ds (data structures) shared object file.
*/
int main(int argc, char** argv)
{
    helloOpenBEST();
    
    geoPoint* gp= createGeoPoint3(10, 12, 14);
    display(gp);
    
    matrix3 m= createMatrix3(1, 2, 1, 3, 1, 2, 1, 2, 1);
    vector3 v= createVector3(1, 1, 2);
    
    displayM(&m);
    displayV(&v);
    
    vector3 r= mpv(m,v);
    displayV(&r);
    
    return 0;
}