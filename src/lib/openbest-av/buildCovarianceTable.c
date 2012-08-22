#include "openbest-av/buildCovarianceTable.h"
#include "openbest-ds/mathFunctions.h"

void buildCovarianceTable(geoPoint2** locations, int n_locations, berkeleyAverageOptions* options, float** weights, int* n_weights, float* nugget)
{
    tprintf("Begin of Build Covariance Table\n");

    tprintf("%f %f %f\n", locations[0]->latitude, locations[0]->longitude, locations[0]->elevation);

    // Precomputed monthly covariance information
    real* p= options->correlationParameters;
    int n_p= options->n_correlationParameters;
    real maxd= options->correlationLimitDistance;

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

    // Eliminate station locations with no usable data -- original comment
    // I cant see how the collapsing of three vectors to a matrix eliminates anything

    real* R= rnalloc(n_targx*3);
    for ( i= 0; i < n_targx; ++i )
    {
        R[i*3 + 0]= targ_x[i];
        R[i*3 + 1]= targ_y[i];
        R[i*3 + 2]= targ_z[i];
    }

    int lenR= n_targx;

    float tmp1, tmp2, tmp3;
    *weights= (float*)malloc(sizeof(float)*(lenR*lenR));
    *n_weights= lenR;
    for ( j= 0; j < lenR; ++j )
    {
        for ( i= 0; i < lenR; ++i )
        {
            tmp1= R[j*3 + 0] - R[i*3 + 0];
            tmp1*= tmp1;
            tmp2= R[j*3 + 1] - R[i*3 + 1];
            tmp2*= tmp2;
            tmp3= R[j*3 + 2] - R[i*3 + 2];
            tmp3*= tmp3;
            (*weights)[i*lenR + j]= sqrt(tmp1 + tmp2 + tmp3);
        }
        for ( i= 0; i < lenR; ++i )
            if ( (*weights)[i*lenR + j] <= maxd )
                (*weights)[i*lenR + j]= exp(polyval(p, n_p, (*weights)[i*lenR + j]));
            else
                (*weights)[i*lenR + j]= 0;
    }

    *nugget= 1.0 - exp(polyval(p, n_p, 0));

    free(targ_x);
    free(targ_y);
    free(targ_z);
    free(R);

    tprintf("End of Build Covariance Table\n");
}
