#include <stdio.h>

#include "openbest-ds/init-ds.h"
#include "openbest-ds/optionTable.h"
#include "openbest-io/loadData.h"
#include "openbest-ds/makeSingleValued.h"
#include "openbest-av/scalpelMethods.h"
#include "openbest-av/berkeleyAverage.h"
#include "openbest-av/berkeleyAverageOptions.h"
#include "openbest-ds/printOut.h"
#include "openbest-av/equationSolvers.h"
#include "openbest-av/idealGrid.h"

int boptionsTest(int argc, char** argv)
{
    berkeleyAverageOptions* bao= createBerkeleyAverageOptionsQuick();
    berkeleyAverageOptionsConsistency(bao);
    return 0;
}

int displayBAOTest(int argc, char** argv)
{
    berkeleyAverageOptions* bao= createBerkeleyAverageOptionsQuick();
    displayBAO(bao);
    return 0;
}

int berkeleyAverageTest(int argc, char** argv)
{
    initDS();

    stationSite2p* ss;
    int n_stationSite2;
    stationElement2p* se;
    int n_stationElement2;

    loadData(&ss, &n_stationSite2, &se, &n_stationElement2);

    int j;
    for ( j= 0; j < n_badFlags; ++j )
        printf("%d ", iBadFlags[j]);

    berkeleyAverageOptions* bao= createBerkeleyAverageOptionsQuick();

    int i;
    stationElement2p tmp;
    tprintf("Making dataset to be single valued\n");
    for ( i= 0; i < n_stationElement2; ++i )
    {
        tmp= makeSingleValued(se[i], NULL, 0);
        destroySE2(se[i]);
        se[i]= tmp;
    }
    tprintf("Converting to single valued finished\n");

    berkeleyAverage(&se, &n_stationElement2, &ss, &n_stationSite2, bao);

    destroySE2V(se, n_stationElement2);
    destroySS2V(ss, n_stationSite2);

    finalizeDS();

    destroyBAO(bao);

    return 0;
}

int eqsolveTest(int argc, char** argv)
{
    double a_data[] = { 0.18, 0.60, 0.57, 0.96,
                               0.41, 0.24, 0.99, 0.58,
                               0.14, 0.30, 0.97, 0.66,
                               0.51, 0.13, 0.19, 0.85,
                                0.2, 0.2, 0.3, 0.2 };

    double b_data[] = { 1.0, 2.0, 3.0, 4.0, 5.0 };

    double x[4];

    solveLinEq(a_data, 5, 4, b_data, x);

    int i;
    for ( i= 0; i < 4; ++i )
        printf("%f ", x[i]);

    return 0;
}

int eqsolve2Test(int argc, char** argv)
{
    double a_data[] = { 1, 1, 1,
                        2, 3, 4,
                        3, 5, 2,
                        4, 2, 5,
                        5, 4, 3};

    double b_data[] = { -10, -3,
                        12, 14,
                        14, 12,
                        16, 16,
                        18, 16};

    double x[10];

    solveLinEqHD(a_data, 5, 3, b_data, 2, x);

    printf("\n");
    int i;
    for ( i= 0; i < 6; ++i )
        printf("%f ", x[i]);


    return 0;
}

int eqsolve3Test(int argc, char** argv)
{
    const int n= 251;
    double a_data[n*n];
    double a2_data[n*n];
    double a3_data[n*n];

    double b_data[n];
    double b2_data[n];

    int i;
    for ( i= 0; i < n*n; ++i )
    {
        a_data[i]= (double)(rand()%10000)/(double)(rand()%20000);
        if ( a_data[i] > 1 )
            a_data[i]= 1.0/a_data[i];
        if ( a_data[i] < 0.0001)
            a_data[i]= 0;
        a3_data[i]= a2_data[i]= a_data[i];
    }
    for ( i= 0; i < n; ++i )
        b2_data[i]= b_data[i]= (double)(rand()%10000)/(double)(rand()%20000);

    /*a_data[0]= 1;
    a_data[1]= 2;
    a_data[2]= 3;
    a_data[3]= 4;*/

    double x[n];
    double x2[n];

    solveLinEq(a_data, n, n, b_data, x);

    for ( i= 0; i < n; ++i )
        printf("%f ", x[i]);

    printf("\n");
    solveLinEqSquare2(a2_data, n, n, b2_data, n, 1);

    for ( i= 0; i < n; ++i )
        printf("%f ", b2_data[i]);

    printf("\n\n");
    int j;
    for ( i= 0; i < n; ++i )
    {
        double tmp= 0;
        for ( j= 0; j < n; ++j )
        {
            tmp+= b2_data[j]*a3_data[i*n + j];
        }
        printf("%f - %f\n", tmp, b_data[i]);
    }

    printf("\n\n");
    for ( i= 0; i < n; ++i )
    {
        double tmp= 0;
        for ( j= 0; j < n; ++j )
        {
            tmp+= x[j]*a3_data[i*n + j];
        }
        printf("%f - %f\n", tmp, b_data[i]);
    }

    return 0;
}

int idealGridTest(int argc, char** argv)
{
    real* latitude;
    real* longitude;
    int n_latitude;
    int n_longitude;

    idealGrid(100, &latitude, &n_latitude, &longitude, &n_longitude);

    printf("%d\n", n_latitude);
    int i;
    for ( i= 0; i < n_latitude; ++i )
        printf("%f %f\n", latitude[i], longitude[i]);

    free(latitude);
    free(longitude);

    return 0;
}

int conditionNumberTest(int argc, char** argv)
{
    double a[]= {2.0, 0.2, 1.8,
                 0.2, 2.4, 0.5,
                 1.8, 0.5, 0.1};
    double cn= conditionNumberEstimateD(a, 3, 3);
    tprintf("condition number: %f\n");
    return 0;
}

int main(int argc, char** argv)
{
    optionTable* ot= createOptionTable();

    setPrefix(ot, "openbest averaging tests");
    addUsage(ot, "test-av <option-name> arg1 arg2 ...");

    bool boptions= false;
    bool displaybao= false;
    bool berkeleyav= false;
    bool solveeq= false;
    bool solveeq2= false;
    bool solveeq3= false;
    bool idealgrid= false;
    bool condnum= false;

    int err;

    addOption(ot, "--boptions", OPTION_BOOL, (char*)&boptions, 0, "BerkeleyAverageOptions test");
    addOption(ot, "--displaybao", OPTION_BOOL, (char*)&displaybao, 0, "displays the berkeleyAverageOptoins object");
    addOption(ot, "--berkeleyav", OPTION_BOOL, (char*)&berkeleyav, 0, "berkeleyAverage test");
    addOption(ot, "--solveeq", OPTION_BOOL, (char*)&solveeq, 0, "equation solver test");
    addOption(ot, "--solveeq2", OPTION_BOOL, (char*)&solveeq2, 0, "equation solver test 2");
    addOption(ot, "--solveeq3", OPTION_BOOL, (char*)&solveeq3, 0, "equation solver test 3");
    addOption(ot, "--idealgrid", OPTION_BOOL, (char*)&idealgrid, 0, "ideal grid test");
    addOption(ot, "--condnum", OPTION_BOOL, (char*)&condnum, 0, "condition number");

    if ( processArgs(ot, &argc, argv) )
    {
        destroyOptionTable(ot);
        return -1;
    }

    if ( boptions )
        err= boptionsTest(argc, argv);
    else if ( displaybao )
        err= displayBAOTest(argc, argv);
    else if ( berkeleyav )
        err= berkeleyAverageTest(argc, argv);
    else if ( solveeq )
        err= eqsolveTest(argc, argv);
    else if ( solveeq2 )
        err= eqsolve2Test(argc, argv);
    else if ( solveeq3 )
        err= eqsolve3Test(argc, argv);
    else if ( idealgrid )
        err= idealGridTest(argc, argv);
    else if ( condnum )
        err= conditionNumberTest(argc, argv);

    destroyOptionTable(ot);

    return err;
}
