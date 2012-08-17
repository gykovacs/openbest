#include "openbest-av/collapseLocations.h"
#include "openbest-ds/memFunctions.h"
#include <stdlib.h>

geoPoint2* centerGP2NV(geoPoint2** v, int n_v, int* indeces, int n_indeces)
{
    int i, j, k;
    real x, y, z;
    x= y= z= 0;
    for ( i= 0; i < n_indeces; ++i )
    {
        x+= v[indeces[i]]->x;
        y+= v[indeces[i]]->y;
        z+= v[indeces[i]]->z;
    }
    x/= n_indeces;
    y/= n_indeces;
    z/= n_indeces;

    real d= sqrt(x*x + y*y + z*z);
    x/= d;
    y/= d;
    z/= d;

    real lat= 90 - acos(z)*180.0/M_PI;
    real longitude= atan2(y, x)*180.0/M_PI;

    return createGeoPoint2N(lat, longitude);
}

void collapseLocations(geoPoint2** locations, int n_locations, double max_move, geoPoint2p** locationsOut, int* n_locationsOut, int** indecesOut, int* n_indecesOut)
{
    if ( max_move < 0 )
        max_move= 50;

    tprintf("Performing Location Collapse\n");

    int i, j, k;

    tprintf("Build Distance Table with %d locations and max move %f km\n", n_locations, max_move);

    bool* mask= bnalloc(n_locations*n_locations);
    int n_mask= n_locations;
    int len_D= n_locations;
    setb(mask, n_locations, false);

    real* x= rnalloc(n_locations);
    real* y= rnalloc(n_locations);
    real* z= rnalloc(n_locations);
    int n_x, n_y, n_z;
    n_x= n_y= n_z= n_locations;

    for ( i= 0; i < n_locations; ++i )
    {
        x[i]= locations[i]->x;
        y[i]= locations[i]->y;
        z[i]= locations[i]->z;
    }

    real tmpx, tmpy, tmpz;
    for ( k= 0; k < n_x; ++k )
    {
        for ( j= k+1; j < n_x; ++j )
        {
            tmpx= x[k] - x[j];
            tmpx*= tmpx;
            tmpy= y[k] - y[j];
            tmpy*= tmpy;
            tmpz= z[k] - z[j];
            tmpz*= tmpz;
            mask[k*n_locations + j]= ((tmpx + tmpy + tmpz) < max_move*max_move ? true : false);
            //printf("%f,%f ", sqrt(tmpx + tmpy + tmpz), max_move*2);
        }
    }

    for ( k= 0; k < n_locations; ++k )
        for ( j= 0; j <= k; ++j )
            mask[k*n_locations + j]= mask[j*n_locations + k];

    for ( k= 0; k < n_locations; ++k )
        mask[k*n_locations + k]= true;

    tprintf("End of Build Distance Table\n");
    tprintf("Begin Build Target List\n");

    int* multiple= inalloc(n_locations);
    for ( i= 0; i < n_locations; ++i )
    {
        multiple[i]= 0;
        for ( j= 0; j < n_locations; ++j )
            multiple[i]+= mask[i*n_locations + j];
    }

    printf("multiple: \n");
    for ( i= 0; i < n_locations; ++i )
        printf("%d ", multiple[i]);
    printf("\n");

    bool* bad= bnalloc(len_D);
    setb(bad, len_D, false);
    bool* kill= bad;

    int* indices= inalloc(n_locations);
    for ( i= 0; i < n_locations; ++i )
        indices[i]= i;

    int** index_list= (int**)malloc(sizeof(int*)*len_D);
    int* n_index_list= (int*)malloc(sizeof(int)*len_D);

    /*for ( i= 0; i < len_D; ++i )
    {
        index_list[i]= (int*)malloc(sizeof(int)*1000);
        index_list[i][0]= i;
        n_index_list[i]= 1;
    }*/

    // Repeatedly loop of location list removing closely associated groups
    // until all locations are separated by at least the min separation distance.

    int next;
    int* fk= inalloc(len_D);
    int n_fk;

    maxIA2(multiple, len_D, &next, fk, &n_fk);
    int* f= inalloc(len_D);
    int n_f;
    int sum= 0;
    geoPoint2** targets= (geoPoint2**)malloc(sizeof(geoPoint2*)*n_locations);
    for ( i= 0; i < n_locations; ++i )
        targets[i]= createGeoPoint2C(locations[i]);
    int n_targets= n_locations;
    geoPoint2* target;
    real* dd= rnalloc(len_D);
    bool* sel= bnalloc(len_D);
    bool* I= bnalloc(len_D);
    int* demult= inalloc(len_D);

    while ( next > 1 )
    {
        tprintf("loop: next - %d\n", next);
        n_f= 0;
        for ( j= 0; j < len_D; ++j )
            if ( mask[fk[0]*len_D + j] && !bad[j] )
            {
                f[n_f++]= j;
                I[j]= true;
            }
            else
                I[j]= false;

        target= centerGP2NV(targets, n_targets, f, n_f);
        for ( i= 0; i < n_f; ++i )
            dd[i]= distanceGP2(target, targets[f[i]]);

        for ( i= 0; i < n_f; ++i )
            sel[i]= dd[i] < max_move;
/*        for ( i= 0; i < n_f; ++i )
            printf("%f,%f ", dd[i], max_move);
        printf("\n");
        for ( i= 0; i < n_f; ++i )
            printf("%d ", sel[i]);
        printf("\n");*/

        for ( i= 0; i < n_f; ++i )
            if ( !sel[i] )
                I[f[i]]= false;

        sum= 0;
        for ( i= 0; i < n_f; ++i )
            sum+= sel[i];

        if ( sum <= 1 )
        {
            if ( sum == 0 )
                for ( i= 0; i < n_f; ++i )
                    I[f[i]]= true;

            seti(demult, len_D, 0);

            for ( i= 0; i < n_f; ++i )
                for ( j= 0; j < len_D; ++j )
                    demult[j]+= mask[j*len_D + f[i]];

            for ( i= 0; i < n_f; ++i )
                for ( j= 0; j < len_D; ++j )
                    mask[j*len_D + f[i]]= false;

            for ( i= 0; i < len_D; ++i )
                multiple[i]-= demult[i];
            for ( i= 0; i < len_D; ++i )
                if ( I[i] )
                    multiple[i]= 0;

            maxIA2(multiple, len_D, &next, fk, &n_fk);
            continue;
        }
        else if ( sum == 2 && n_f > 1 )
        {
            target= centerGP2NV(targets, n_targets, f, n_f);
        }

        for ( i= 0; i < len_D; ++i )
            if ( I[i] )
                bad[i]= true;

        n_f= 0;
        for ( i= 0; i < len_D; ++i )
            if ( I[i] )
                f[n_f++]= i;

        for ( i= 1; i < n_f; ++i )
            kill[f[i]]= true;

        targets[f[0]]= target;

        seti(demult, len_D, 0);

        for ( i= 0; i < n_f; ++i )
            for ( j= 0; j < len_D; ++j )
                demult[j]+= mask[j*len_D + f[i]];

        for ( i= 0; i < n_f; ++i )
            for ( j= 0; j < len_D; ++j )
                mask[j*len_D + f[i]]= false;

        for ( i= 0; i < len_D; ++i )
            multiple[i]-= demult[i];
        for ( i= 0; i < len_D; ++i )
            if ( I[i] )
                multiple[i]= 0;

        maxIA2(multiple, len_D, &next, fk, &n_fk);
    }

    /*for ( i= 0; i < len_D; ++i )
        printf("%d ", kill[i]);
    printf("\n");*/

    for ( i= 0; i < len_D; ++i )
        if ( kill[i] )
        {
            targets[i]= NULL;
            //index_list[i]= -1;
        }

    j= 0;
    for ( i= 0; i < len_D; ++i )
    {
        if ( targets[i] != NULL )
        {
            targets[j++]= targets[i];
            //index_list[j++]= index_list[i];
        }
    }
    n_targets= j;

    tprintf("End of Build Target List\n");

    tprintf("number of targets: %d\n", n_targets);

    tprintf("Begin Generate Reference Table\n");

    targets= (geoPoint2**)realloc(targets, (sizeof(geoPoint2*))*(n_targets+1));
    targets[n_targets]= createGeoPoint2N();
    ++n_targets;

    real* tx= rnalloc(n_targets);
    real* ty= rnalloc(n_targets);
    real* tz= rnalloc(n_targets);

    for ( i= 0; i < n_targets; ++i )
    {
        tx[i]= targets[i]->x;
        ty[i]= targets[i]->y;
        tz[i]= targets[i]->z;
    }

    // Now that we have a list of effective station locations to use, assign
    // each actual location to its nearest effective location.

    real* templ= rnalloc(n_targets);
    real mn;
    int* fkr= inalloc(n_targets);
    int n_fkr;
    for ( k= 0; k < len_D; ++k )
    {
        for ( i= 0; i < n_targets; ++i )
        {
            tmpx= x[k] - tx[i];
            tmpx*= tmpx;
            tmpy= y[k] - ty[i];
            tmpy*= tmpy;
            tmpz= z[k] - tz[i];
            tmpz*= tmpz;

            templ[i]= sqrt(tmpx + tmpy + tmpz);
        }

        minRA2(templ, n_targets, &mn, fkr, &n_fkr);
        indices[k]= fkr[0];
    }

    int* un;
    int n_un;
    uniqueIAN(indices, len_D, &un, &n_un);

    int* tmpA= inalloc(n_targets);
    for ( i= 0; i < n_targets; ++i )
        tmpA[i]= i;

    int* bad2= inalloc(n_targets);
    int n_bad2;
    setdiffIA(tmpA, n_targets, un, n_un, bad2, &n_bad2);

    for ( k= n_bad2-1; k >= 0; --k )
    {
        for ( i= 0; i < len_D; ++i )
            if ( indices[i] >= bad2[k] )
                indices[i]--;
    }

    j= 0;
    for ( i= 0; i < n_targets; ++i )
    {
        for ( k= 0; k < n_bad2; ++k )
            if ( bad2[k] == i )
                break;
        if ( k == n_bad2 )
            targets[j++]= targets[i];
    }

    n_targets= j;

    *indecesOut= indices;
    *n_indecesOut= len_D;
    *locationsOut= targets;
    *n_locationsOut= n_targets;

    tprintf("freeing allocated memory\n");
    tprintf("%d total locations used\n", n_targets);

    free(x);
    free(y);
    free(z);
    free(mask);
    free(multiple);
    free(bad);
    free(index_list);
    free(n_index_list);
    free(fk);
    free(f);
    free(dd);
    free(sel);
    free(I);
    free(demult);
    free(tx);
    free(ty);
    free(tz);
    free(templ);
    free(fkr);
    free(un);
    free(tmpA);
    free(bad2);

    tprintf("End of Generate Reference Table\n");

    tprintf("End of Performing Location Collapse\n");
}
