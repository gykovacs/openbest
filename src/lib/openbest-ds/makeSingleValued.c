#include "openbest-ds/makeSingleValued.h"
#include "openbest-ds/memFunctions.h"
#include <string.h>
#include <limits.h>

stationElement2* makeSingleValued(stationElement2* se, int* bf, int n)
{
    if ( isSingleValued(se) )
        return createSE2NC(se);

    stationElement2* st1, *st2;
    int* n_result;
    st1= structureMerge(se);
    st1= mergeCore(st1, bf, n, "merge_any", &st2, &n_result);
    free(n_result);
    return st2;
}

stationElement2* structureMerge(stationElement2* se)
{
    //Unused in MATLAB code because of makeSingleValued only supports single input.
    ///TODO: the function must be implemented if it is called from other places than makeSingleValued
    return se;
}

void getIndexValueS(short* M, int* mask, int size1, int c, short* v, bool* est)
{
    short* v1= (short*)snalloc(size1);
    short* v2= (short*)snalloc(size1);

    int i, j, k;
    short minTmp, maxTmp;
    for ( i= 0; i < size1; ++i )
    {
        minTmp= maxTmp= M[i*c];
        for ( j= 0; j < c; ++j )
        {
            if ( minTmp > M[i*c + j] )
                minTmp= M[i*c + j];
            if ( maxTmp < M[i*c + j] )
                maxTmp= M[i*c + j];
        }
        v1[i]= maxTmp;
        v2[i]= minTmp;
    }

    //dea();
    short* va= (short*)snalloc(size1);
    for ( i= 0; i < size1; ++i )
        va[i]= v1[i];

    int* f= (int*)inalloc(size1);
    int n_f= 0;

    //deb();
    for ( i= 0; i < size1; ++i )
        if ( (v1[i] != v2[i]) && !(v1[i] < 0 && v2[i] < 0) )
            f[n_f++]= i;
    for ( i= 0; i < n_f; ++i )
        va[f[i]]= -9999;

    //dec();
    for ( i= 0; i < size1; ++i )
        for ( j= 0; j < c; ++j )
            if ( mask[i*c + j] )
                M[i*c + j]= -10000;

    //ded();
    for ( i= 0; i < size1; ++i )
    {
        minTmp= maxTmp= M[i*c];
        for ( j= 0; j < c; ++j )
        {
            if ( minTmp > M[i*c + j] && M[i*c + j] > -10000 )
                minTmp= M[i*c + j];
            if ( maxTmp < M[i*c + j] && M[i*c + j] > -10000 )
                maxTmp= M[i*c + j];
        }
        v1[i]= maxTmp;
        v2[i]= minTmp;
    }

    //dee();
    short* vb= snalloc(size1);
    for ( i= 0; i < size1; ++i )
        vb[i]= v1[i];

    n_f= 0;
    for ( i= 0; i < size1; ++i )
        if ( (v1[i] != v2[i]) && !(v1[i] < 0 && v2[i] < 0) )
            f[n_f++]= i;
    for ( i= 0; i < n_f; ++i )
        vb[f[i]]= -9999;

    for ( i= 0; i < size1; ++i )
        v[i]= vb[i];

    for ( i= 0; i < size1; ++i )
        est[i]= false;

    //def();
    for ( i= 0; i < size1; ++i )
    {
        if ( vb[i] < -9999 )
            est[i]= true;
    }

    //deg();
    for ( i= 0; i < size1; ++i )
    {
        if ( va[i] < -9999 && v[i] < -9999)
            est[i]= true;
    }

    //deh();
    for ( i= 0; i < size1; ++i )
        if ( v[i] < -9999 )
        {
            v[i]= va[i];
            est[i]= false;
        }

    //dei();
    free(va);
    free(vb);
    free(v1);
    free(v2);
    free(f);
}

void getIndexValueC(char* M, int* mask, int size1, int c, char* v, bool* est)
{
    char* v1= cnalloc(size1);
    char* v2= cnalloc(size1);

    int i, j, k;
    char minTmp, maxTmp;
    //dea();
    for ( i= 0; i < size1; ++i )
    {
        minTmp= maxTmp= M[i*c];
        for ( j= 0; j < c; ++j )
        {
            if ( minTmp > M[i*c + j] )
                minTmp= M[i*c + j];
            if ( maxTmp < M[i*c + j] )
                maxTmp= M[i*c + j];
        }
        v1[i]= maxTmp;
        v2[i]= minTmp;
    }

    //deb();
    char* va= cnalloc(size1);
    for ( i= 0; i < size1; ++i )
        va[i]= v1[i];

    //dec();
    int* f= inalloc(size1);
    int n_f= 0;

    for ( i= 0; i < size1; ++i )
        if ( (v1[i] != v2[i]) && !(v1[i] < 0 && v2[i] < 0) )
            f[n_f++]= i;
    for ( i= 0; i < n_f; ++i )
        va[f[i]]= -99;

    for ( i= 0; i < size1; ++i )
        for ( j= 0; j < c; ++j )
            if ( mask[i*c + j] )
                M[i*c + j]= -100;

    //ded();
    for ( i= 0; i < size1; ++i )
    {
        minTmp= maxTmp= M[i*c];
        for ( j= 0; j < c; ++j )
        {
            if ( minTmp > M[i*c + j] && M[i*c + j] > -100 )
                minTmp= M[i*c + j];
            if ( maxTmp < M[i*c + j] && M[i*c + j] > -100 )
                maxTmp= M[i*c + j];
        }
        v1[i]= maxTmp;
        v2[i]= minTmp;
    }

    //dee();
    char* vb= cnalloc(size1);
    for ( i= 0; i < size1; ++i )
        vb[i]= v1[i];

    //dea(); dea();
    n_f= 0;
    for ( i= 0; i < size1; ++i )
        if ( (v1[i] != v2[i]) && !(v1[i] < 0 && v2[i] < 0) )
            f[n_f++]= i;
    //deb(); deb();
    for ( i= 0; i < n_f; ++i )
        vb[f[i]]= -99;

    //dec(); dec();
    for ( i= 0; i < size1; ++i )
        v[i]= vb[i];

    //ded(); ded();
    for ( i= 0; i < size1; ++i )
        est[i]= false;

    //def();
    for ( i= 0; i < size1; ++i )
    {
        if ( vb[i] < -99 )
            est[i]= true;
    }

    for ( i= 0; i < size1; ++i )
    {
        if ( va[i] < -99 && v[i] < -99)
            est[i]= true;
    }

    for ( i= 0; i < size1; ++i )
        if ( v[i] < -99 )
        {
            v[i]= va[i];
            est[i]= false;
        }

    //deg();
    free(va);
    free(vb);
    free(v1);
    free(v2);
    free(f);
}

void maskOverFlagsF(flag_t* flags, int* mask, int size1, int c, int l)
{
    int i, j, k;
    for ( i= 0; i < size1; ++i )
        for ( k= 0; k < c; ++k )
        {
            for ( j= k; j < c*l; j+= l)
                if ( mask[i*c + k] )
                    flags[i*c + j]= 0;
        }
}

void maskOverFlagsI(int* flags, int* mask, int size1, int c, int l)
{
    int i, j, k;
    for ( i= 0; i < size1; ++i )
        for ( k= 0; k < c; ++k )
        {
            for ( j= k; j < c*l; j+= l)
                if ( mask[i*c + k] )
                    flags[i*c + j]= 0;
        }
}

flag_t* anyismember(flag_t** flags, char* n_flags, int n_n_flags, int* bf, int n)
{
    qsortIA(bf, n);
    flag_t* res= fnalloc(n_n_flags);
    setf(res, n_n_flags, false);
    int i, j, k;
    for ( i= 0; i < n_n_flags; ++i )
        for ( j= 0; j < n_flags[i]; ++j )
            for ( k= 0; k < n && bf[k] <= flags[i][j]; ++k )
                if ( flags[i][j] == bf[k] )
                {
                    res[i]= true;
                    break;
                }
    return res;
}

void cleanFlags(stationElement2p se)
{

}

stationElement2* mergeCore(stationElement2* se, int* bf, int n, char* action, stationElement2p* result, int** n_result)
{
    int z;
    /*for ( z= 0; z < n; ++z )
        printf("%d - %s\n", bf[z], badFlags[z]);*/

    int repeats= 1;
    int* num_conflict;
    int* tob_conflict;
    bool* num_est;
    bool* tob_est;

    int command;

    if ( strcmp(action, "raw") == 0 )
        return se;
    else if ( strcmp(action, "merge_any") == 0 )
        command= 1;
    else if ( strcmp(action, "merge_consistent") == 0 )
        command= 2;
    else if ( strcmp(action, "average_similar") == 0 )
        command= 3;
    else if ( strcmp(action, "average_dissimilar") == 0 )
        command= 4;
    else if ( strcmp(action, "remove_duplicates") == 0 )
        command= 5;
    else
        eprintf("Action instruction not understood in mergeCore");

    ///TODO change frequency in stationElement2 to array

    char* freq_type= lookupKeysPKT(frequencyTypesPKT, se->frequency)[0];

    real* target_dates;
    int n_dates;

    int size0= se->n_dates;

    int n_target_dates;
    uniqueRAN(se->dates, se->n_dates, &target_dates, &n_target_dates);

    stationElement2* result_st= createSE2NC(se);

    free(result_st->dates);

    real* dates= copyRA(se->dates, se->n_dates);
    n_dates= se->n_dates;
    result_st->dates= target_dates;
    result_st->n_dates= n_target_dates;

    dates= (real*)realloc(dates, sizeof(real)*(n_dates + 1));
    dates[n_dates]= -FLT_MAX;
    n_dates++;

    // length of data series
    int ld= n_dates - 1;

    int c= modeRFA(dates, n_dates);

    //tprintf("c: %d\n", c);
    if ( c == 1 )
    {
        /**result= se;
        *n_result= 1;*/
        stationElement2p res= createSE2NC(se);
        *result= res;
        *n_result= inalloc(1);
        (*n_result)[0]= 1;
        return res;
    }

    //************************
    // allocate data holders
    //************************
    real* data= rnalloc(ld*c);  // dimensions: ld x c, stride: c, type: real
    set(data, ld*c, 0);
    real* unc= copyRA(data, ld*c);  //dimensions: ld x c, stride: c, type: real
    short* num= snalloc(ld*c);   // dimensions: ld x c, stride: c, type: short
    sets(num, ld*c, SHRT_MAX);
    char* tob= cnalloc(ld*c);   // dimensions: ld x c, stride: c, type: char
    setc(tob, ld*c, CHAR_MAX);
    flag_t* flagged= fnalloc(ld*c); // dimensions: ld x c, stride: c, type: flag_t
    setf(flagged, ld*c, false);
    flag_t* orig_flagged= anyismember(se->flags, se->n_flags, se->n_n_flags, bf, n); // dimensions: ld x 1, stride: 1


    int lf= maxC(se->n_flags, se->n_n_flags);

    flag_t* flags= fnalloc(ld*c*lf);    // dimensions: ld x c*lf, stride: c*lf

    int ls= maxC(se->n_sources, se->n_n_sources);

    int* source= inalloc(ld*c*ls);      // dimensions: ld x c*ls, stride: c*ls

    //tprintf("lf, ls: %d %d\n", lf, ls);

    //*************************
    // initialize  data holders
    //*************************
    int i, j, k, stride, s_flags, s_source, s_data;
    stride= c;
    s_flags= c*lf;
    s_source= c*ls;
    s_data= c;
    for ( i= 0; i < ld; ++i )
    {
        data[i*stride + 0]= se->data[i];
        unc[i*stride + 0]= se->uncertainty[i];
        num[i*stride + 0]= se->num_measurements[i];
        tob[i*stride + 0]= se->time_of_observation[i];

        for ( j= 0; j < lf; ++j )
            if ( j < se->n_flags[i] )
                flags[i*s_flags + j]= se->flags[i][j];
            else
                flags[i*s_flags + j]= 0;
        for ( j= 0; j < ls; ++j )
            if ( j < se->n_sources[i] )
                source[i*s_source + j]= se->sources[i][j];
            else
                source[i*s_source + j]= 0;
        flagged[i*stride + 0]= orig_flagged[i];
    }

    ///TODO: implement special processing to accelerate certain cases for command={5,2}

    //tprintf("ld, c: %d %d\n", ld, c);
    int* counts= inalloc(ld*c);     // dimensions: ld x c, stride: c
    //deg(); deg();
    seti(counts, ld*c, 0);
    //printf("%d\n", n_dates); fflush(stdout);
    int* f= inalloc(n_dates);
    int n_f= 0;
    if ( command == 5 )
        ; ///TODO
    else
    {
        for ( i= 0; i < n_dates - 1; ++i )
        {
            if ( dates[i] != dates[i+1] )
                f[n_f++]= i;
        }
    }

    /*for ( i= 0; i < n_f; ++i )
        printf("%d ", f[i]);
    printf("\n");*/

    flag_t* used= fnalloc(ld + 1);  // dimensions: (ld + 1) x 1, stride: 1
    setf(used, ld+1, false);

    for ( i= 0; i < n_f; ++i )
        used[f[i]]= true;
    for ( i= 0; i < n_f; ++i )
        counts[f[i]*stride+0]= 1;

    for ( k= 2; k <= c; ++k )
    {
        //ded(); printf("%d ", k); fflush(stdout);
        if ( command == 5 )
            ; //TODO
        else
        {
            n_f= 0;
            for ( i= 0; i < n_dates - k; ++i )
                if ( (dates[i] != dates[i+k]) && !used[i])
                    f[n_f++]= i;
        }

        /*printf("k - %d: ", k);
        for ( i= 0; i < n_f; ++i )
            printf("%d ", f[i]);
        printf("\n");*/

        if ( isemptyI(f, n_f) )
            continue;

        for ( i= 0; i < n_f; ++i )
        {
            data[(f[i] + k - 1)*s_data + k - 1]= data[f[i]];
            num[(f[i] + k - 1)*s_data + k - 1]= num[f[i]];
            tob[(f[i] + k - 1)*s_data + k - 1]= tob[f[i]];
            unc[(f[i] + k - 1)*s_data + k - 1]= unc[f[i]];
            flagged[(f[i] + k - 1)*s_data + k - 1]= flagged[f[i]];

            for ( j= 0; j < lf; ++j )
                flags[(f[i] + k - 1)*s_flags + j + lf*(k-1)]= flags[f[i]*s_flags+ j];
            for ( j= 0; j < ls; ++j )
                source[(f[i] + k - 1)*s_source + j + ls*(k-1)]= source[f[i]*s_source+ j];

            used[f[i]]= true;
            counts[(f[i] + k - 1 )*s_data + k - 1]= 1;
        }
    }

    /*tprintf("n_f, c: %d %d\n", n_f, c);
    for ( i= 0; i < ld; ++i )
    {
        printf("%d %f %d: ", i, dates[i], used[i]);
        for ( j= 0; j < c; ++j )
            printf("%d ", counts[i*s_data + j]);
        printf("\n");
    }*/

    // Remove the lines that have now been stacked -- original comment

    int sum;
    n_f= 0;
    for ( i= 0; i < ld; ++i )
    {
        sum= 0;
        for ( j= 0; j < c; ++j )
            sum+= counts[i*c + j];
        if ( sum > 0 )
        {
            f[n_f++]= i;
        }
    }

    for ( i= 0; i < n_f; ++i )
    {
        dates[n_f]= dates[i];
        for ( j= 0; j < c; ++j )
        {
            data[i*stride + j]= data[f[i]*stride + j];
            num[i*stride + j]= num[f[i]*stride + j];
            unc[i*stride + j]= unc[f[i]*stride + j];
            tob[i*stride + j]= tob[f[i]*stride + j];
            flagged[i*stride + j]= flagged[f[i]*stride + j];
            counts[i*stride + j]= counts[f[i]*stride + j];
        }
        for ( k= 0; k < c*lf; ++k )
        {
            flags[i*c*lf + k]= flags[f[i]*c*lf + k];
        }
        for ( k= 0; k < c*ls; ++k )
        {
            source[i*c*ls + k]= source[f[i]*c*ls + k];
        }
    }

    /*printf("counts: \n");
    for ( i= 0; i < n_f; ++i )
    {
        printf("%f %d : ", dates[i], i);
        for ( j= 0; j < c; ++j )
            printf("%d ", counts[i*stride + j]);
        printf("\n");
    }

    printf("data: \n");
    for ( i= 0; i < n_f; ++i )
    {
        printf("%f %d : ", dates[i], i);
        for ( j= 0; j < c; ++j )
            printf("%f ", data[i*stride + j]);
        printf("\n");
    }*/

    data= realloc(data, n_f*c*sizeof(real));
    num= realloc(num, n_f*c*sizeof(short));
    unc= realloc(unc, n_f*c*sizeof(real));
    tob= realloc(tob, n_f*c*sizeof(char));
    flags= realloc(flags, n_f*lf*c*sizeof(flag_t));
    source= realloc(source, n_f*ls*c*sizeof(int));
    flagged= realloc(flagged, n_f*c*sizeof(flag_t));
    counts= realloc(counts, n_f*c*sizeof(int));

    //printf("C: %d\n", c);
    int size1= n_f;
    n_f= 0;
    for ( i= 0; i < size1; ++i )
    {
        sum= 0;
        //printf("%d,%d - ", i, c);
        for ( j= 0; j < c; ++j )
        {
            //printf("%d ", counts[i*c + j]);
            sum+= counts[i*c + j];
        }
        if ( sum > 1 )
            f[n_f++]= i;
        //printf("\n");
    }

    int size2= n_f;
    real* data2= rnalloc(size2*c); // dimensions: size2 x c, stride: c
    short* num2= snalloc(size2*c);  // dimensions: size2 x c, stride: c
    int* counts2= inalloc(size2*c);
    real* unc2= rnalloc(size2*c);   // dimensions: size2 x c, stride: c
    char* tob2= cnalloc(size2*c);   // dimensions: size2 x c, stride: c
    flag_t* flags2= fnalloc(size2*lf*c);    // dimensions: size2 x (c*lf), stride: c*lf
    int* source2= inalloc(size2*ls*c);      // dimensions: size2 x (c*ls), stride: c*ls
    flag_t* flagged2= fnalloc(size2*c);     // dimensions: size2 x c, stride: c

    /*for ( i= 0; i < n_f; ++i )
        printf("%d ", f[i]);
    printf("\n");*/

    n_f= 0;
    for ( i= 0; i < size2; ++i )
    {
        for ( j= 0; j < c; ++j )
        {
            data2[i*stride + j]=    data[f[i]*stride + j];
            num2[i*stride + j]=     num[f[i]*stride + j];
            counts2[i*stride + j]=  counts[f[i]*stride + j];
            unc2[i*stride + j]= unc[f[i]*stride + j];
            tob2[i*stride + j]= tob[f[i]*stride + j];
            flagged2[i*stride + j]= flagged[f[i]*stride + j];
        }
        for ( k= 0; k < c*lf; ++k )
        {
            flags2[i*s_flags + k]= flags[f[i]*s_flags + k];
        }
        for ( k= 0; k < c*ls; ++k )
        {
            source2[i*s_source + k]= source[f[i]*s_source + k];
        }
    }

    /*for ( i= 0; i < size2; ++i )
    {
        for ( j= 0; j < c; ++j )
            printf("%f ", data2[i*stride + j]);
        printf("\n");
    }*/

    /*printf("flagged2: \n");
    for ( i= 0; i < size2; ++i )
    {
        for ( j= 0; j < c; ++j )
            printf("%d ", flagged2[i*stride + j]);
        printf("\n");
    }*/

    /*for ( i= 0; i < size2; ++i )
        printf("%f %d %d\n", data2[i*stride + 0], num2[i*stride + 0], counts2[i*stride + 0]);*/

    bool* no_flags= (bool*)malloc(sizeof(bool)*size2);
    bool* all_flags= (bool*)malloc(sizeof(bool)*size2);
    bool* some_flags= (bool*)malloc(sizeof(bool)*size2);

    int sum_counts;
    int sum_flagged;
    for ( i= 0; i < size2; ++i )
    {
        sum_counts= 0;
        sum_flagged= 0;
        for ( j= 0; j < c; ++j )
        {
            sum_flagged+= flagged2[c*i + j];
            sum_counts+= counts2[c*i + j];
        }
        if ( sum_flagged == 0 )
            no_flags[i]= true;
        else
            no_flags[i]= false;

        if ( sum_flagged == sum_counts )
            all_flags[i]= true;
        else
            all_flags[i]= false;

        if ( !no_flags[i] && !all_flags[i] )
            some_flags[i]= true;
        else
            some_flags[i]= false;
    }

    int* mask= inalloc(size2*c);
    for ( i= 0; i < size2*c; ++i )
        mask[i]= 0;
    //if ( !isemptyI(flagged2, size2) )
    {
        for ( i= 0; i < size2; ++i )
            if ( some_flags[i] )
                for ( j= 0; j < c; ++j )
                    mask[i*c + j]= (flagged2[i*c + j] == 1);

        /*for ( i= 0; i < size2; ++i )
            //if ( some_flags[i] )
            {
                for ( j= 0; j < c; ++j )
                    printf("%d ", mask[i*c + j]);
                printf("\n");
            }*/
    }

    real* data_high= rnalloc(size2*c);
    real* data_low= rnalloc(size2*c);

    int* consistent;
    //printf("command: \n"); fflush(stdout);
    switch (command)
    {
        case 1: ;
        case 5: ;
            for ( i= 0; i < size2; ++i )
                for ( j= 0; j < c; ++j )
                    if ( mask[i*c + j] )
                    {
                        data2[i*c + j]= 0;
                        unc2[i*c + j]= 0;
                        counts2[i*c + j]= 0;
                    }

            for ( i= 0; i < size2; ++i )
                for ( j= 0; j < c; ++j )
                {
                    data_high[i*c + j]= data2[i*c + j] + unc2[i*c + j];
                    if ( ! counts2[i*c + j] )
                        data_high[i*c + j]= FLT_MAX;
                    data_low[i*c + j]= data2[i*c + j] - unc2[i*c + j];
                    if ( ! counts2[i*c + j] )
                        data_low[i*c + j]= -FLT_MAX;
                }

            /*for ( i= 0; i < size2; ++i )
            {
                for ( j= 0; j < c; ++j )
                {
                    printf("%f,%f,%f ", data2[i*c + j], data_high[i*c + j], data_low[i*c + j]);
                }
                printf("\n");
            }*/

            rangeResolver(data, unc, &consistent, f, size2, data_high, data_low, c, c, 0);

            /*for ( i= 0; i < size1; ++i )
            {
                for ( j= 0; j < c; ++j )
                {
                    printf("%f,%f ", data[i*c + j], unc[i*c + j]);
                }
                printf("\n");
            }*/

            real* unc3= rnalloc(size2*c);
            for ( i= 0; i < size2; ++i )
                for ( j= 0; j < c; ++j )
                    unc3[i*c + j]= unc2[i*c + j];
            for ( i= 0; i < size2; ++i )
                for ( j= 0; j < c; ++j )
                    if ( !counts[i*c + j] )
                        unc3[i*c + j]= FLT_MAX;

            real* min_unc= rnalloc(size2);
            real minTmp;
            for ( i= 0; i < size2; ++i )
            {
                minTmp= unc3[i*c];
                for ( j= 0; j < c; ++j )
                    if ( minTmp > unc3[i*c + j] )
                        minTmp= unc3[i*c + j];
                min_unc[i]= minTmp;
            }

            for ( i= 0; i < size2; ++i )
                if ( unc[f[i]*c] < min_unc[i] )
                    unc[f[i]*c]= min_unc[i];

            if ( command == 5 )
                for ( i= 0; i < size1; ++i )
                    result_st->dates[i]= dates[i];

            //printf("%d %d\n", size1, size2);
            for ( i= 0; i < size1; ++i )
            {
                result_st->data[i]= data[i*c];
                result_st->uncertainty[i]= unc[i*c];
            }

            /*printf("%d %d %d\n", result_st->n_dates, result_st->n_data, result_st->n_uncertainty);
            for ( i= 0; i < result_st->n_dates; ++i )
                printf("%f %f %f\n", result_st->dates[i], result_st->data[i], result_st->uncertainty[i]);*/

            free(unc3);
            free(min_unc);

        case 2: ; // TODO on demand
        case 3: ; // TODO on demand
        case 4: ; // TODO on demand;
    }

    switch (command)
    {
        case 1: ;
        case 2: ;
        case 3: ;
        case 4: ;
        case 5: ;
        short* num_all= snalloc(size2);
        num_est= (bool*)bnalloc(size2);
        char* tob_all= cnalloc(size2);
        tob_est= (bool*)bnalloc(size2);

        getIndexValueS(num2, mask, size2, c, num_all, num_est);
        getIndexValueC(tob2, mask, size2, c, tob_all, tob_est);

        num_conflict= inalloc(size2);
        tob_conflict= inalloc(size2);

        for ( i= 0; i < size2; ++i )
        {
            if ( num_all[i] == -9999 )
            {
                num_conflict[i]= 1;
            }
            else
                num_conflict[i]= 0;

            if ( tob_all[i]= -99 )
                tob_conflict[i]= 1;
            else
                tob_conflict[i]= 0;
        }

        for ( i= 0; i < size2; ++i )
        {
            num[f[i]*c]= num_all[i];
            tob[f[i]*c]= tob_all[i];
        }

        maskOverFlagsF(flags2, mask, size2, c, lf);
        maskOverFlagsI(source2, mask, size2, c, ls);

        for ( i= 0; i < size2; ++i )
        {
            for ( j= 0; j < ls; ++j )
            {
                if ( source2[i*c*ls + j] )
                {
                    source[f[i]*c*ls + j]= source2[i*c*ls + j];
                }
            }
            //deb();
            for ( j= 0; j < lf; ++j )
            {
                if ( flags2[i*c*lf + j] )
                {
                    flags[f[i]*c*lf + j]= flags2[i*c*lf + j];
                }
            }
        }

        //dea();
        for ( i= 0; i < size1; ++i )
        {
            result_st->num_measurements[i]= num[i*c];
            result_st->time_of_observation[i]= tob[i*c];
            result_st->n_sources[i]= 0;
            //printf("%d-", result_st->n_flags[i]);
            result_st->n_flags[i]= 0;
            //deb();
            for ( j= 0; j < ls; ++j )
                if ( source[i*c*ls + j] )
                    result_st->n_sources[i]++;
            result_st->sources[i]= realloc(result_st->sources[i], sizeof(flag_t)*result_st->n_sources[i]);
            k= 0;
            for ( j= 0; j < ls; ++j )
                if ( source[i*c*ls + j] )
                    result_st->sources[i][k++]= source[i*c*ls + j];

            for ( j= 0; j < lf; ++j )
                if ( flags[i*c*lf + j] )
                    result_st->n_flags[i]++;
            result_st->flags[i]= realloc(result_st->flags[i], sizeof(flag_t)*result_st->n_flags[i]);
            k= 0;
            for ( j= 0; j < lf; ++j )
                if ( flags[i*c*lf + j] )
                    result_st->flags[i][k++]= flags[i*c*lf + j];
            //ded();
        }
        //dea();

        for ( i= size1; i < size0; ++i )
        {
            free(result_st->sources[i]);
            free(result_st->flags[i]);
        }

        free(num_all);
        free(tob_all);

    }

    // Flags to be added

    // Avearge includes missing values (allowed for some kinds of averages)

    int n_f2= 0;
    int* f2= inalloc(size1);
    for ( i= 0; i < size1; ++i )
    {
        sum= 0;
        for ( j= 0; j < c; ++j )
            sum+= counts[i*c + j];
        if ( sum > 1 )
            f2[n_f2++]= i;
    }

    char name[100];
    for ( i= 0; i < n_f2; ++i )
    {
        strcpy(name, freq_type);
        switch (command)
        {
        case 1:
            strcat(name, "MULTIPLE_VALUES_MERGED");
            break;
        case 2:
            strcat(name, "CONSISTENT_VALUES_MERGED");
            break;
        case 3:
            strcat(name, "AVERAGED_SIMILAR_VALUES");
            break;
        case 4:
            strcat(name, "AVERAGED_DISSIMILAR_VALUES");
            break;
        case 5:
            strcat(name, "DUPLICATES_REMOVED");
            break;
        }
        result_st->flags[f2[i]]= (flag_t*)realloc(result_st->flags[f2[i]], (result_st->n_flags[f2[i]]+1)*sizeof(flag_t));
        result_st->flags[f2[i]][result_st->n_flags[f2[i]]++]= lookupValuePKT(dataFlagsPKT, name);
    }

    n_f2= 0;
    for ( i= 0; i < size1; ++i )
    {
        sum= 0;
        for ( j= 0; j < c; ++j )
            sum+= counts[i*c + j];
        if ( sum < repeats )
            f2[n_f2++]= i;
    }

    for ( i= 0; i < n_f2; ++i )
    {
        strcpy(name, freq_type);
        strcat(name, "_AVERAGE_MISSING_VALUES");
        result_st->flags[f2[i]]= (flag_t*)realloc(result_st->flags[f2[i]], (result_st->n_flags[f2[i]]+1)*sizeof(flag_t));
        result_st->flags[f2[i]][result_st->n_flags[f2[i]]++]= lookupValuePKT(dataFlagsPKT, name);
    }

    for ( i= 0; i < size2; ++i )
    {
        if ( some_flags[i] )
        {
            strcpy(name, freq_type);
            strcat(name, "_MERGER_BAD_FLAGGED_VALUE_DROPPED");
            result_st->flags[f[i]]= (flag_t*)realloc(result_st->flags[f[i]], (result_st->n_flags[f[i]]+1)*sizeof(flag_t));
            result_st->flags[f[i]][result_st->n_flags[f[i]]++]= lookupValuePKT(dataFlagsPKT, name);
        }
        if ( num_conflict[i] )
        {
            strcpy(name, freq_type);
            strcat(name, "_MERGER_CONFLICT_NUM");
            result_st->flags[f[i]]= (flag_t*)realloc(result_st->flags[f[i]], (result_st->n_flags[f[i]]+1)*sizeof(flag_t));
            result_st->flags[f[i]][result_st->n_flags[f[i]]++]= lookupValuePKT(dataFlagsPKT, name);
        }
        if ( tob_conflict[i] )
        {
            strcpy(name, freq_type);
            strcat(name, "_MERGER_CONFLICT_TOB");
            result_st->flags[f[i]]= (flag_t*)realloc(result_st->flags[f[i]], (result_st->n_flags[f[i]]+1)*sizeof(flag_t));
            result_st->flags[f[i]][result_st->n_flags[f[i]]++]= lookupValuePKT(dataFlagsPKT, name);
        }
        if ( num_est[i] )
        {
            strcpy(name, freq_type);
            strcat(name, "_MERGER_CONFLICT_NUM");
            result_st->flags[f[i]]= (flag_t*)realloc(result_st->flags[f[i]], (result_st->n_flags[f[i]]+1)*sizeof(flag_t));
            result_st->flags[f[i]][result_st->n_flags[f[i]]++]= lookupValuePKT(dataFlagsPKT, name);
        }
        if ( tob_est[i] )
        {
            strcpy(name, freq_type);
            strcat(name, "_MERGER_CONFLICT_NUM");
            result_st->flags[f[i]]= (flag_t*)realloc(result_st->flags[f[i]], (result_st->n_flags[f[i]]+1)*sizeof(flag_t));
            result_st->flags[f[i]][result_st->n_flags[f[i]]++]= lookupValuePKT(dataFlagsPKT, name);
        }
        if ( consistent[i] )
        {
            strcpy(name, freq_type);
            strcat(name, "_MERGED_CONSISTENT");
            result_st->flags[f[i]]= (flag_t*)realloc(result_st->flags[f[i]], (result_st->n_flags[f[i]]+1)*sizeof(flag_t));
            result_st->flags[f[i]][result_st->n_flags[f[i]]++]= lookupValuePKT(dataFlagsPKT, name);
        }
        if ( command == 1 && !consistent[i] )
        {
            strcpy(name, freq_type);
            strcat(name, "_MERGED_INCONSISTENT");
            result_st->flags[f[i]]= (flag_t*)realloc(result_st->flags[f[i]], (result_st->n_flags[f[i]]+1)*sizeof(flag_t));
            result_st->flags[f[i]][result_st->n_flags[f[i]]++]= lookupValuePKT(dataFlagsPKT, name);
        }
        /*if ( avg_inconsistent[i] )
        {
            strcpy(name, freq_type);
            strcat(name, "_MERGED_CONSISTENT");
            result_st->flags[f[i]]= (flag_t*)realloc(result_st->flags[f[i]], (result_st->n_flags[f[i]]+1)*sizeof(flag_t));
            result_st->flags[f[i]][result_st->n_flags[f[i]]++]= lookupValuePKT(dataFlags, name);
        }*/
    }

    //tprintf("ALMOST ENDED\n");
    //return se;
    *n_result= inalloc(size1);
    for ( i= 0; i < size1; ++i )
    {
        sum= 0;
        for ( j= 0; j < c; ++j )
            sum+= counts[i*c + j];
        (*n_result)[i]= sum;
    }

    //tprintf("size after making single valued: %d\n", size1);
    //getchar();

    result_st->n_dates= size1;
    result_st->n_data= size1;
    result_st->n_uncertainty= size1;
    result_st->n_num_measurements= size1;
    result_st->n_time_of_observation= size1;
    result_st->n_n_flags= size1;
    result_st->n_n_sources= size1;

    cleanFlags(result_st);

    free(data);
    free(unc);
    free(tob);
    free(num);
    free(flagged);
    free(flags);
    free(source);
    free(data2);
    free(unc2);
    free(tob2);
    free(num2);
    free(flagged2);
    free(flags2);
    free(source2);
    free(dates);
    free(orig_flagged);
    free(counts);
    free(counts2);
    free(used);
    free(f);
    free(f2);
    free(num_est);
    free(tob_est);
    free(num_conflict);
    free(tob_conflict);
    free(mask);
    free(no_flags);
    free(all_flags);
    free(some_flags);
    free(consistent);
    free(data_high);
    free(data_low);

    *result= result_st;
    return result_st;
}


void rangeResolver(real* data, real* unc, int** consistent, int* f, int n_f, real* max_table, real* min_table, int c, int cc, int kk)
{
    int i, j, k;

    /*printf("rangeResolver: %d %d %d\n", n_f, c, kk);
    for ( i= 0; i < n_f; ++i )
        for ( j= 0; j < c; ++j )
            printf("%f,%f,%f,%f\n", data[f[i]], unc[f[i]], max_table[i*c + j], min_table[i*c + j]);*/
    //getchar();

    real* d_high= rnalloc(n_f);
    real* d_low= rnalloc(n_f);

    real extr_min, extr_max;
    for ( i= 0; i < n_f; ++i )
    {
        extr_min= extr_max= max_table[i*c + 0];
        for ( j= 0; j < c; ++j )
        {
            if ( max_table[i*c + j] < extr_min )
                extr_min= max_table[i*c + j];
            if ( min_table[i*c + j] > extr_max )
                extr_max= min_table[i*c + j];
        }
        d_high[i]= extr_min;
        d_low[i]= extr_max;
    }

    int* good= inalloc(n_f);
    for ( i= 0; i < n_f; ++i )
        if ( d_high[i] >= d_low[i] )
        {
            good[i]= true;
        }
        else
            good[i]= false;

    *consistent= good;

    for ( i= 0; i < n_f; ++i )
        if ( good[i] )
        {
            data[f[i]*cc + kk]= (d_high[i] + d_low[i])/2;
            unc[f[i]*cc + kk]= (d_high[i] - d_low[i])/2;
        }

    int all_good= 0;
    for ( i= 0; i < n_f; ++i )
        all_good+= good[i];
    if ( all_good == n_f )
    {
        //tprintf("all good\n");
        free(d_high);
        free(d_low);
        return;
    }

    int bad= n_f - all_good;

    //tprintf("bad: %d\n", bad);
    real* max_table2= rnalloc(bad*(c-1));
    real* min_table2= rnalloc(bad*(c-1));

    real* val2= rnalloc(bad*(c));
    real* err2= rnalloc(bad*(c));
    set(val2, bad*(c), 0);
    set(err2, bad*(c), 0);

    int a= 0;
    int* select= inalloc(c-1);
    int* consistent2;

    for ( k= 0; k < c; ++k )
    {
            for ( j= 0; j < k; ++j )
                select[j]= j;
            for ( j= k+1; j < c; ++j )
                select[j-1]= j;

            a= 0;
            for ( j= 0; j < n_f; ++j )
                if ( !good[j] )
                {
                    for ( i= 0; i < c-1; ++i )
                    {
                        max_table2[a*(c-1) + i]= max_table[j*c + select[i]];
                        min_table2[a*(c-1) + i]= min_table[j*c + select[i]];
                    }
                    ++a;
                }

            int* f2= inalloc(bad);

            for ( j= 0; j < bad; ++j )
                f2[j]= j;

            //tprintf("recursive call %d...\n", a);
            rangeResolver(val2, err2, &consistent2, f2, bad, max_table2, min_table2, c-1, c, k);
            //tprintf("recursion finished...\n");

            for ( i= 0; i < bad; ++i )
                for ( j= 0; j < c-1; ++j )
                {
                    max_table2[i*(c-1) + j]= val2[i*(c-1)+j] + err2[i*(c-1)+j];
                    min_table2[i*(c-1) + j]= val2[i*(c-1)+j] - err2[i*(c-1)+j];
                }

            for ( i= 0; i < bad; ++i )
            {
                extr_min= extr_max= max_table2[i*(c-1) + 0];
                for ( j= 0; j < c-1; ++j )
                {
                    if ( min_table2[i*(c-1) + j] < extr_min )
                        extr_min= min_table2[i*(c-1) + j];
                    if ( max_table2[i*(c-1) + j] > extr_max )
                        extr_max= max_table2[i*(c-1) + j];
                }
                d_high[i]= extr_max;
                d_low[i]= extr_min;
            }

            for ( i= 0; i < n_f; ++i )
                if ( !good[i] )
                {
                    data[f[i]*cc + kk]= (d_high[i] + d_low[i])/2.0;
                    unc[f[i]*cc + kk]= (d_high[i] - d_low[i])/2.0;
                }
            free(f2);
            free(consistent2);
    }
    free(d_high);
    free(d_low);
    free(max_table2);
    free(min_table2);
    free(val2);
    free(err2);
    free(select);
}
