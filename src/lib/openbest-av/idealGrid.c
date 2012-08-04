#include "openbest-ds/memFunctions.h"
#include "openbest-av/idealGrid.h"
#include "openbest-av/equationSolvers.h"

#include <math.h>
#include <string.h>
#include <stdio.h>

void idealGrid(int num, real** latitudeIO, int* n_latitude, real** longitudeIO, int* n_longitude)
{
    double target_area= 4.0*M_PI/num;

    double d_length= sqrt(target_area);

    int ysteps= round(M_PI/d_length);

    double ylength= M_PI/ysteps;

    double* ypos= dnalloc(ysteps);

    int i, j, k;
    for ( i= 0; i < ysteps; ++i )
    {
        if ( i*ylength + ylength/2.0 > M_PI - ylength/2.0 )
            break;
        ypos[i]= ylength/2.0 + i*ylength;
    }

    double* xwidth= dnalloc(ysteps);
    int n_xwidth= ysteps;

    for ( i= 0; i < ysteps; ++i )
        xwidth[i]= 2*M_PI*sin(ypos[i]);

    double* xsteps= dnalloc(ysteps);
    int n_xsteps= ysteps;
    for ( i= 0; i < ysteps; ++i )
        xsteps[i]= round(xwidth[i]/d_length);

    // Determining latitude divisions to enforce the equal area requirement
    // depends on solving a linear algebra problem outlined below

    double* A= dnalloc(n_xsteps * (n_xsteps + 1)); // dimensions: n_xsteps x (n_xsteps + 1); stride: n_xsteps + 1
    for ( k= 0; k < n_xsteps * (n_xsteps + 1); ++k )
        A[k]= 0;
    for ( k= 0; k < n_xsteps; ++k )
    {
        A[k*(n_xsteps + 1) + k]= 1.0/xsteps[k];
        A[k*(n_xsteps + 1) + k + 1]= -1.0/xsteps[k];
    }

    for ( k= 0; k < n_xsteps*(n_xsteps + 1); ++k )
        A[k]= A[k]*2*M_PI;

    double* O= dnalloc(n_xsteps * 2);
    for ( k= 0; k < n_xsteps; ++k )
    {
        O[k*2 + 0]= A[k*(n_xsteps + 1) + 0];
        O[k*2 + 1]= A[k*(n_xsteps + 1) + n_xsteps];
    }

    double* A2= dnalloc(n_xsteps * (n_xsteps - 1));
    for ( k= 0; k < n_xsteps; ++k )
        for ( j= 0; j < n_xsteps - 1; ++j )
        {
            A2[k*(n_xsteps - 1) + j]= A[k*(n_xsteps + 1) + j + 1];
        }

    free(A);
    A= A2; //dimensions: n_xsteps x
    double sum_xsteps= 0;
    for ( i= 0; i < n_xsteps; ++i )
        sum_xsteps+= xsteps[i];

    double* B= dnalloc(n_xsteps);
    for ( i= 0; i < n_xsteps; ++i )
        B[i]= 2*M_PI/sum_xsteps;

    // in the next line of the Matlab code, O is multiplied by [sin(pi/2); sin(-pi/2)], but this is actually [+1 -1]
    double* C= dnalloc(n_xsteps);
    for ( i= 0; i < n_xsteps; ++i )
        C[i]= B[i] - (O[i*2 + 0] - O[i*2 + 1]);

    // solution of the matrix equation using GNU gsl
    double* X= dnalloc(n_xsteps-1);
    solveLinEq(A, n_xsteps, n_xsteps-1, C, X);

    // X2 stores the latitudes defining grid cell boundaries
    double* X2= dnalloc(n_xsteps + 1);
    X2[0]= sin(M_PI/2.0);
    memcpy(X2+1, X, sizeof(double)*(n_xsteps-1));
    X2[n_xsteps]= sin(-M_PI/2.0);
    int n_X2= n_xsteps + 1;

    real* latitude= rnalloc(sum_xsteps);
    set(latitude, sum_xsteps, 0);
    real* longitude= copyRA(latitude, sum_xsteps);

    double cnt= 0;
    for ( k= 0; k < n_X2-1; ++k )
    {
        for ( j= (int)cnt; j <= (int)(cnt + xsteps[k])-1; ++j )
        {
            latitude[j]= (asin(X2[k]) + asin(X2[k+1]))/2.0*180.0/M_PI;
            longitude[j]= -180.0 + 360.0/(2*xsteps[k]) + (j-cnt)*360.0/xsteps[k];
        }
        cnt+= xsteps[k];
    }
    *latitudeIO= rnalloc(sum_xsteps);
    *longitudeIO= rnalloc(sum_xsteps);

    for ( i= 0; i < sum_xsteps; ++i )
    {
        (*latitudeIO)[i]= latitude[i];
        (*longitudeIO)[i]= longitude[i];
    }

    *n_latitude= sum_xsteps;
    *n_longitude= sum_xsteps;

    free(longitude);
    free(latitude);
    free(X2);
    free(X);
    free(C);
    free(B);
    free(A);
    free(O);
    free(ypos);
    free(xsteps);
    free(xwidth);
}
