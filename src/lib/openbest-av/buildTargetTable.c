#include "openbest-av/buildTargetTable.h"
#include "openbest-ds/mathFunctions.h"
#include "openbest-ds/memFunctions.h"

void buildTargetTable(geoPoint2** locations, int n_locations, geoPoint2** map, int n_map, berkeleyAverageOptions* options, float** target_map, int* n_target_map, int** nearest, int* n_nearest)
{
    tprintf("Begin Build Target Map\n");

    // Data locations
    real* targ_x= rnalloc(n_locations);
    real* targ_y= rnalloc(n_locations);
    real* targ_z= rnalloc(n_locations);

    int i, j, k;

    for ( i= 0; i < n_locations; ++i )
    {
        targ_x[i]= locations[i]->x;
        targ_y[i]= locations[i]->y;
        targ_z[i]= locations[i]->z;
    }

    int n_targx, n_targy, n_targz;
    n_targx= n_targy= n_targz= i;

    // locations for numerical integral grid
    real* X= rnalloc(n_map);
    real* Y= rnalloc(n_map);
    real* Z= rnalloc(n_map);

    for ( i= 0; i < n_map; ++i )
    {
        X[i]= map[i]->x;
        Y[i]= map[i]->y;
        Z[i]= map[i]->z;
    }
    int n_X, n_Y, n_Z;
    n_X= n_Y= n_Z= n_map;

    // Precomputed monthly covariance information
    real* p= options->correlationParameters;
    int n_p= options->n_correlationParameters;
    real maxd= options->correlationLimitDistance;

    // Eliminate station locations with no usable data
    real* R= rnalloc(n_targx*3);
    for ( i= 0; i < n_targx; ++i )
    {
        R[i*3 + 0]= targ_x[i];
        R[i*3 + 1]= targ_y[i];
        R[i*3 + 2]= targ_z[i];
    }

    int lenR= n_targx;

    *target_map= (float*)malloc(sizeof(float)*n_X*lenR);

    float dd;
    float tmp1, tmp2, tmp3;
    for ( j= 0; j < n_X; ++j )
    {
        for ( i= 0; i < lenR; ++i )
        {
            tmp1= X[j] - R[i*3 + 0];
            tmp1*= tmp1;
            tmp2= Y[j] - R[i*3 + 1];
            tmp2*= tmp2;
            tmp3= Z[j] - R[i*3 + 2];
            tmp3*= tmp3;
            dd= sqrt(tmp1 + tmp2 + tmp3);
            //printf("%f ", X[j]);
            if ( dd <= maxd )
                (*target_map)[j*lenR + i]= exp(polyval(p, n_p, dd));
            else
                (*target_map)[j*lenR + i]= 0;
        }
    }

    *nearest= inalloc(lenR);
    for ( i= 0; i < lenR; ++i )
        (*nearest)[i]= (*target_map)[i];
    for ( i= 0; i < lenR; ++i )
        for ( j= 0; j < n_X; ++j )
        {
            if ( (*target_map)[j*lenR + i] > (*target_map)[(*nearest)[i]*lenR + i] )
                (*nearest)[i]= j;
        }

    *n_target_map= n_X;
    *n_nearest= lenR;

    tprintf("End of Build Target Map\n");
}
