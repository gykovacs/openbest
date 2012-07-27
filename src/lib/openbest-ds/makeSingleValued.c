#include "openbest-ds/makeSingleValued.h"

stationElement2* makeSingleValued(stationElement2* se, int* bf, int n)
{
    if ( isSingleValued(se) )
        return se;

    stationElement2* st1, *st2;
    st1= structureMerge(se);
    st2= mergeCore(st1, bf, n, "merge_any", NULL, NULL);
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

stationElement2* mergeCore(stationElement2* se, int* bf, int n, char* action, stationElement2p* result, int* n_result)
{
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

    char* freq_type= lookupKeysPKT(frequencyTypesPKT, se->frequency)[0];

    real* target_dates;
    int n_target_dates;
    uniqueRAN(se->dates, se->n_dates, &target_dates, &n_target_dates);

    stationElement2* result_st= createSE2NC(se);

    free(result_st->dates);
    result_st->dates= target_dates;
    result_st->n_dates= n_target_dates;

    result_st->dates= (real*)realloc(result_st->dates, sizeof(real)*(result_st->n_dates + 1));
    result_st->dates[result_st->n_dates]= -FLT_MAX;
    result_st->n_dates++;

    // length of data series
    int ld= result_st->n_dates - 1;

    int c= modeRFA(result_st->dates, result_st->n_dates);

    tprintf("c: %d\n", c);
    if ( c == 1 )
    {
        /**result= se;
        *n_result= 1;*/
        return se;
    }

    //************************
    // allocate data holders
    //************************
    real* data= rnalloc(ld*c);  // dimensions: ld x c, stride: c
    set(data, ld*c, 0);         // dimensions: ld x c, stride: c
    real* unc= copyRA(data, ld*c);  //dimensions: ld x c, stride: c
    real* num= rnalloc(ld*c);   // dimensions: ld x c, stride: c
    set(num, ld*c, FLT_MAX);    // dimensions: ld x c, stride: c
    real* tob= rnalloc(ld*c);   // dimensions: ld x c, stride: c
    set(tob, ld*c, FLT_MAX);    // dimensions: ld x c, stride: c
    flag_t* flagged= fnalloc(ld*c); // dimensions: ld x c, stride: c
    setf(flagged, ld*c, false); // dimensions: ld x c, stride: c
    flag_t* orig_flagged= anyismember(se->flags, se->n_flags, se->n_n_flags, bf, n); // dimensions: ld x 1, stride: 1


    int lf= maxC(se->n_flags, se->n_n_flags);

    flag_t* flags= fnalloc(ld*c*lf);    // dimensions: ld x c*lf, stride: c*lf

    int ls= maxC(se->n_sources, se->n_n_sources);

    int* source= inalloc(ld*c*ls);      // dimensions: ld x c*ls, stride: c*ls

    tprintf("%d %d\n", lf, ls);

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

    tprintf("%d %d\n", ld, c);
    int* counts= (int*)inalloc(ld*c);     // dimensions: ld x c, stride: c
    deg(); deg();
    seti(counts, ld*c, 0);
    printf("%d\n", result_st->n_dates); fflush(stdout);
    int* f= (int*)inalloc(result_st->n_dates);
    int n_f= 0;
    if ( command == 5 )
        ; ///TODO
    else
    {
        for ( i= 0; i < result_st->n_dates - 1; ++i )
        {
            if ( result_st->dates[i] != result_st->dates[i+1] )
                f[n_f++]= i;
        }
    }

    flag_t* used= fnalloc(ld + 1);  // dimensions: (ld + 1) x 1, stride: 1
    setf(used, ld+1, false);

    for ( i= 0; i < n_f; ++i )
        used[f[i]]= true;
    for ( i= 0; i < n_f; ++i )
        counts[f[i]*stride+0]= 1;

    for ( k= 2; k <= c; ++k )
    {
        ded(); printf("%d ", k); fflush(stdout);
        if ( command == 5 )
            ; //TODO
        else
        {
            n_f= 0;
            for ( i= 0; i < result_st->n_dates - k; ++i )
                if ( result_st->dates[i] != result_st->dates[i+k] && !used[ld + 1 - k])
                    f[n_f++]= i;
        }

        if ( isemptyF(f, n_f) )
            continue;

        for ( i= 0; i < n_f; ++i )
        {
            data[(f[i] + k - 1)*s_data + k]= data[f[i]];
            num[(f[i] + k - 1)*s_data + k]= num[f[i]];
            tob[(f[i] + k - 1)*s_data + k]= tob[f[i]];
            unc[(f[i] + k - 1)*s_data + k]= unc[f[i]];
            flagged[(f[i] + k - 1)*s_data + k]= flagged[f[i]];

            for ( j= 0; j < lf; ++j )
                flags[(f[i] + k - 1)*s_flags + j + lf*(k-1)]= flags[f[i]+ j];
            for ( j= 0; j < ls; ++j )
                source[(f[i] + k - 1)*s_flags + j + ls*(k-1)]= source[f[i]+ j];

            used[f[i]]= true;
            counts[(f[i] + k - 1)*s_data + k]= 1;
        }
    }

    tprintf("%d %d\n", n_f, c);
    for ( i= 0; i < n_f; ++i )
    {
        for ( j= 0; j < c; ++j )
            printf("%d ", counts[i*s_data + j]);
        printf("\n");
    }
    return se;

    // Remove the lines that have now been stacked -- original comment
    /*setf(f, ld, 0);
    int sum;
    for ( i= 0; i < ld; ++i )
    {
        sum= 0;
        for ( j= 0; j < c; ++j )
            sum+= counts[i*c + j];
        if ( sum > 0 )
        {
            f[i]= true;
        }
    }
    real* dates= result_st->dates;

    n_f= 0;
    for ( i= 0; i < ld; ++i )
    {
        if ( f[i] )
        {
            dates[n_f]= dates[i];
            for ( j= 0; j < c; ++j )
            {
                data[n_f*stride + j]= data[i*stride + j];
                num[n_f*stride + j]= num[i*stride + j];
                unc[n_f*stride + j]= unc[i*stride + j];
                tob[n_f*stride + j]= tob[i*stride + j];
                flagged[n_f*stride + j]= flagged[i*stride + j];
                counts[n_f*stride + j]= counts[i*stride + j];
            }
            for ( k= 0; k < c*lf; ++k )
            {
                flags[n_f*lf + k]= flags[i*lf + k];
                source[n_f*stride + k]= source[i*stride + k];
            }
            ++n_f;
        }
    }

    data= realloc(data, n_f*sizeof(real));
    num= realloc(num, n_f*sizeof(short));
    unc= realloc(unc, n_f*sizeof(real));
    tob= realloc(tob, n_f*sizeof(char));
    flags= realloc(flags, n_f*lf*sizeof(flag_t));
    source= realloc(source, n_f*ls*sizeof(int));
    flagged= realloc(flagged, n_f*sizeof(flag_t));
    counts= realloc(counts, n_f*sizeof(int));

    int size1= n_f;
    setf(f, n, 0);
    n_f= 0;
    for ( i= 0; i < size1; ++i )
    {
        sum= 0;
        for ( j= 0; j < c; ++j )
            sum+= counts[i*c + j];
        if ( sum > 1 )
        {
            f[i]= true;
            ++n_f;
        }
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

    n_f= 0;
    for ( i= 0; i < size1; ++i )
    {
        if ( f[i] )
        {
            for ( j= 0; j < c; ++j )
            {
                data2[n_f*stride + j]= data[i*stride + j];
                num2[n_f*stride + j]= num[i*stride + j];
                counts2[n_f*stride + j]= num[i*stride + j];
                unc2[n_f*stride + j]= unc[i*stride + j];
                to2[n_f*stride + j]= tob[i*stride + j];
                flagged2[n_f*stride + j]= flagged[i*stride + j];
            }
            for ( k= 0; k < c*lf; ++k )
            {
                flags[n_f*lf + k]= flags[i*lf + k];
                source[n_f*stride + k]= source[i*stride + k];
            }
            ++n_f;
        }
    }

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

    bool* mask= (bool*)malloc(sizeof(bool)*size2*c);
    if ( !isempty(flagged2, size2) )
    {
        for ( i= 0; i < size2; ++i )
            if ( some_flags[i] )
                if ( flagged2[i] == 1 )
                    for ( j= 0; j < c; ++j )
                        mask[i*c + j]= flagged2[i*c + j];

    }

    temp_t* data_high= tnalloc(size2*c);
    temp_t* data_low= tnalloc(size2*c);

    bool* consistent;
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
                    if ( ! counts[i*c + j] )
                        data_high[i*c + j]= FLT_MAX;
                    data_low[i*c + j]= data2[i*c + j] - unc2[i*c + j];
                    if ( ! counts[i*c + j] )
                        data_low[i*c + j]= -FLT_MAX;
                }

            rangeResolver(data, unc, &consistent, f, size1, size2, c, data_high, data_low);


        case 2: ; // TODO on demand
        case 3: ; // TODO on demand
        case 4: ; // TODO on demand;
    }*/

    free(data);
    free(unc);
    free(tob);
    free(num);
    free(flagged);
    free(flags);
    free(source);
    return se;
}

void rangeResolver(temp_t* data, real* unc, bool** consistent, bool* f, int size1, int size2, int c, real* max_table, real* min_table)
{
    /*int i, j, k;

    temp_t* val= tnalloc(size2);
    unc* err= rnalloc(size2);

    for ( i= 0; i < size2; ++i )
        val[i]= err[i]= 0;

    temp_t* d_high= tnalloc(size2);
    temp_t* d_low= tnalloc(size2);

    temp_t extr_min, extr_max;
    for ( i= 0; i < size2; ++i )
    {
        extr_min= extr_max= max_table[i*c + 0];
        for ( j= 0; j < c; ++j )
        {
            if ( max_table[i*c + j] < extr_min )
                extr_min= max_table[i*c + j];
            if ( min_table[i*c + j] > extr_max )
                extr_max= max_table[i*c + j];
        }
        d_high[i]= extr_min;
        d_low[i]= extr_max;
    }

    bool* good= bnalloc(size2);
    for ( i= 0; i < size2; ++i )
        if ( d_high[i] >= d_low[i] )
            good[i]= true;
        else
            good[i]= false;

    *consistent= good;

    for ( i= 0; i < size2; ++i )
        if ( good[i] )
        {
            val[i]= (d_high[i] + d_low[i])/2;
            err[i]= (d_high[i] - d_low[i])/2;
        }

    int all_good= 0;
    for ( i= 0; i < size2; ++i )
        all_good+= good[i];
    if ( all_good == size2 )
        return;

    temp_t* max_table2= tnalloc(all_good*c);
    temp_t* min_table2= tnalloc(all_good*c);

    temp_t* val2= tnalloc(all_good*c);
    temp_t* err2= tnalloc(all_good*c);

    int n_2= 0;
    for ( i= 0; i < size2; ++i )
    {
        if ( !good[i] )
        {
            for ( j= 0; j < c; ++j )
                val2[n_2*c + j]= err2[n_2*c + j]= 0;
            for ( j= 0; j < c; ++j )
            {
                max_table2[n_2*c + j]= max_table[i*c + j];
                min_table2[n_2*c + j]= min_table[i*c + j];
            }
        }
    }

    for ( k= 0; k < c; ++k )
    {

    }*/
}
