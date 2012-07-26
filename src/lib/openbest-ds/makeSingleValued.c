#include "openbest-ds/makeSingleValued.h"

stationElement2* makeSingleValued(stationElement2* se, int* bf, int n)
{
    if ( isSingleValued(se) )
        return se;

    stationElement2* st1, *st2;
    st1= structureMerge(se);
    st2= mergeCore(st1, bf, n, "merge_any");
    return st2;
}

stationElement2* structureMerge(stationElement2* se)
{
    //Unused in MATLAB code because of makeSingleValued only supports single input.
    ///TODO: the function must be implemented if it is called from other places than makeSingleValued
    return se;
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

stationElement2* mergeCore(stationElement2* se, int* bf, int n, char* action, stationElement2p* result, int* result_count)
{
    dea();
    int repeats= 1;

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

    deb();
    char* freq_type= lookupKeysPKT(frequencyTypesPKT, se->frequency)[0];
    dec();

    real* target_dates;
    int n_target_dates;
    ded();
    uniqueRAN(se->dates, se->n_dates, &target_dates, &n_target_dates);
    dee();

    stationElement2* result_st= createSE2NC(se);

    free(result_st->dates);
    result_st->dates= target_dates;
    result_st->n_dates= n_target_dates;

    def();
    //displaySE2(se);
    //displaySE2(result_st);

    result_st->dates= (real*)realloc(result_st->dates, sizeof(real)*(result_st->n_dates + 1));
    result_st->dates[result_st->n_dates]= -FLT_MAX;
    result_st->n_dates++;

    int ld= result_st->n_dates - 1;

    int c= modeRFA(result_st->dates, result_st->n_dates);
    if ( c == 1 )
    {
        *result= se;
        *n_result= 1;
        return se;
    }

    // initialize data holders
    real* data= rnalloc(ld*c);
    set(data, ld*c, 0);
    real* unc= copyRA(data, ld*c);
    real* num= rnalloc(ld*c);
    set(num, ld*c, FLT_MAX);
    real* tob= rnalloc(ld*c);
    set(tob, ld*c, FLT_MAX);
    flag_t* flagged= fnalloc(ld*c);
    setf(flagged, ld*c, false);
    flag_t* orig_flagged= anyismember(se->flags, se->n_flags, se->n_n_flags, bf, n);

    int lf= maxC(se->n_flags, se->n_n_flags);

    flag_t* flags= fnalloc(ld*c*lf);

    int ls= maxC(se->n_sources, se->n_n_sources);

    int* source= inalloc(ld*c*ls);

    int i, j, k, stride, s_flags, s_source;
    stride= c;
    s_flags= c*lf;
    s_source= c*ls;
    for ( i= 0; i < ld; ++i )
    {
        data[i*stride + 0]= se->data[i];
        unc[i*stride + 0]= se->uncertainty[i];
        num[i*stride + 0]= se->num_measurements[i];
        tob[i*stride + 0]= se->time_of_observation[i];

        for ( j= 0; j < lf; ++j )
            if ( j < se->n_flags[i] )
                flags[i*lf + j]= se->flags[i][j];
            else
                flags[i*lf + j]= 0;
        for ( j= 0; j < lf; ++j )
            if ( j < se->n_sources[i] )
                source[i*s_flags + j]= se->sources[i][j];
            else
                source[i*lf + j]= 0;
        flagged[i*s_source + 0]= orig_flagged[i];
    }

    ///TODO: implement special processing to accelerate certain cases for command={5,2}

    int* counts= inalloc(ld*c);
    seti(counts, ld*c, 0);

    flag_t* f= fnalloc(result_st->n_dates);
    int fn= 0;
    if ( command == 5 )
        ; ///TODO
    else
    {
        for ( i= 0; i < result_st->n_dates - 1; ++i )
            if ( result_st->dates[i] != result_st->dates[i+1] )
                f[fn++]= i;
    }
    f= realloc(f, fn*sizeof(flag_t));

    flag_t* used= fnalloc(ld + 1);
    setf(used, ld+1, false);
    for ( i= 0; i < fn; ++i )
        used[f[i]]= true;
    for ( i= 0; i < fn; ++i )
        counts[f[i]*c+0]= 1;

    for ( k= 1; k < c; ++k )
    {
        if ( command == 5 )
            ; //TODO
        else
        {
            fn= 0;
            f= realloc(f, ld*sizeof(flag_t));
            for ( i= 0; i < result_st->n_dates-k; ++i )
                if ( result_st->dates[i] != result_st->dates[i+k] && !used[ld+1-k])
                    f[fn++]= i;
        }

        if ( isemptyF(f, fn) )
            continue;

        for ( i= 0; i < fn; ++i )
        {
            data[(f[i] + k - 1)*stride + k]= data[f[i]];
            num[(f[i] + k - 1)*stride + k]= num[f[i]];
            tob[(f[i] + k - 1)*stride + k]= tob[f[i]];
            unc[(f[i] + k - 1)*stride + k]= unc[f[i]];
            flagged[(f[i] + k - 1)*stride + k]= flagged[f[i]];
            ///TODO sources
            used[f[i]]= true;
            counts[(f[i] + k - 1)*stride + k]= 1;
        }
    }

    // Remove the lines that have now been stacked -- original comment
    setf(f, ld, 0);
    int sum;
    fn= 0;
    for ( i= 0; i < ld; ++i )
    {
        sum= 0;
        for ( j= 0; j < c; ++j )
            sum+= counts[i*c + j];
        if ( sum > 0 )
        {
            f[i]= true;
            ++fn;
        }
    }
    real* dates= result_st->dates;

    fn= 0;
    for ( i= 0; i < ld; ++i )
    {
        if ( f[i] )
        {
            dates[fn]= dates[i];
            for ( j= 0; j < c; ++j )
            {
                data[fn*stride + j]= data[i*stride + j];
                num[fn*stride + j]= num[i*stride + j];
                unc[fn*stride + j]= unc[i*stride + j];
                tob[fn*stride + j]= tob[i*stride + j];
                flags[fn*lf + j]= flags[i*lf + j];
                //source[fn*stride + j]= data[i*stride + j];
                flagged[fn*stride + j]= flagged[i*stride + j];
                counts[fn*stride + j]= counts[i*stride + j];

            }
            ++fn;
        }
    }

    data= realloc(data, fn*sizeof(real));
    num= realloc(num, fn*sizeof(short));
    unc= realloc(unc, fn*sizeof(real));
    tob= realloc(tob, fn*sizeof(char));
    flags= realloc(flags, fn*lf*sizeof(flag_t));
    flagged= realloc(flagged, fn*sizeof(flag_t));
    counts= realloc(counts, fn*sizeof(int));

    setf(f, n, 0);
    fn= 0;
    for ( i= 0; i < ld; ++i )
    {
        sum= 0;
        for ( j= 0; j < c; ++j )
            sum+= counts[i*c + j];
        if ( sum > 1 )
        {
            f[i]= true;
            ++fn;
        }
    }

    real* data2= rnalloc(fn*c);
    short* num2= snalloc(fn*c);
    real* unc2= rnalloc(fn*c);
    char* tob2= cnalloc(fn*c);
    flag_t* flags2= fnalloc(fn*lf*sizeof(flag_t));
    flag_t* flagged2= fnalloc(fn*c);
    ///TODO source



    return se;
}
