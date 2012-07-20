#include <string.h>
#include <stdlib.h>

#include "openbest-ds/stationElement2.h"
#include "openbest-ds/primaryKeyTables.h"
#include "openbest-ds/memFunctions.h"
#include "openbest-ds/mathFunctions.h"

void displaySE2(stationElement2* se)
{
    printf("stationElement2:\n");
    printf("\t record_type: %d\n", se->record_type);
    printf("\t frequency: %d\n", se->frequency);
    printf("\t site: %d\n", se->site);
    printf("\t dates: %d - ", se->n_dates);
    int i,j;
    for ( i= 0; i < se->n_dates; ++i )
        printf("%.3f ", se->dates[i]);
    printf( "\n\t time of observation: %d - ", se->n_time_of_observation);
    for ( i= 0; i < se->n_time_of_observation; ++i )
        printf("%d ", se->time_of_observation[i]);
    printf("\n\t data: %d - ", se->n_data);
    for ( i= 0; i < se->n_data; ++i )
        printf("%f ", se->data[i]);
    printf("\n\t uncertainty: %d - ", se->n_uncertainty);
    for ( i= 0; i < se->n_uncertainty; ++i )
        printf("%f ", se->uncertainty[i]);
    printf("\n\t n_num_measurements: - %d", se->n_num_measurements);
    for ( i= 0; i < se->n_num_measurements; ++i )
        printf("%d ", se->num_measurements[i]);
    printf("\n\t n_n_flags: %d - \n", se->n_n_flags);
    for ( i= 0; i < se->n_n_flags; ++i )
        for ( j= 0; j < se->n_flags[i]; ++j )
        {
            printf("%f ", se->dates[i]); fflush(stdout);
            printf("%d ", se->n_flags[i]); fflush(stdout);
            printf("%d ", se->flags[i][j]); fflush(stdout);
            printf("%s, ", lookupKeysPKT(dataFlagsPKT, se->flags[i][j])[0]);
            printf("\n"); fflush(stdout);
        }
    printf("\n\t source: %d\n", se->source);
}

stationElement2* createSE2N()
{
    stationElement2* tmp= (stationElement2*)calloc(1,sizeof(stationElement2));
    tmp->dates= NULL;
    tmp->time_of_observation= NULL;
    tmp->data= NULL;
    tmp->uncertainty= NULL;
    tmp->num_measurements= NULL;
    tmp->n_flags= NULL;
    tmp->flags= NULL;
    tmp->record_flags= NULL;
    tmp->primary_record_ids= NULL;
    tmp->record_type= -1;
    tmp->frequency= -1;
    tmp->site= -1;
    tmp->n_dates= 0;
    tmp->n_time_of_observation= 0;
    tmp->n_data= 0;
    tmp->n_uncertainty= 0;
    tmp->n_num_measurements= 0;
    tmp->n_n_flags= 0;
    tmp->source= -1;
    tmp->n_record_flags= 0;
    tmp->n_primary_record_ids= 0;

    tmp->compressed= false;
    tmp->compressedRecordLength= 0;
    tmp->dataLength= 0;
    tmp->compressedRecord= NULL;

    return tmp;
}

stationElement2* createSE2Test()
{
    stationElement2* tmp= createSE2N();
    tmp->dates= rnalloc(3);
    tmp->dates[0]= 1984;
    tmp->dates[1]= 1984.4;
    tmp->dates[2]= 1984.5;

    tmp->record_type= 0;
    tmp->frequency= 0;
    tmp->site= 0;
    tmp->n_dates= 3;
    tmp->n_time_of_observation= 3;
    tmp->n_data= 3;
    tmp->n_uncertainty= 3;
    tmp->n_num_measurements= 3;
    tmp->n_n_flags= 3;
    tmp->source= 0;
    tmp->n_record_flags= 1;
    tmp->n_primary_record_ids= 0;

    tmp->time_of_observation= cnalloc(3);
    tmp->time_of_observation[0]= 2;
    tmp->time_of_observation[1]= 20;
    tmp->time_of_observation[2]= 14;

    tmp->data= tnalloc(3);
    tmp->data[0]= 10.3;
    tmp->data[1]= 14.8;
    tmp->data[2]= 16.0;

    tmp->uncertainty= rnalloc(3);
    tmp->uncertainty[0]= 0.5;
    tmp->uncertainty[1]= 0.2;
    tmp->uncertainty[2]= 0.8;

    tmp->num_measurements= snalloc(3);
    tmp->num_measurements[0]= 3;
    tmp->num_measurements[1]= 10;
    tmp->num_measurements[2]= 8;

    tmp->n_flags= cnalloc(3);
    tmp->n_flags[0]= 2;
    tmp->n_flags[1]= 1;
    tmp->n_flags[2]= 0;

    tmp->flags= (flag_t**)malloc((sizeof(flag_t*))*3);
    tmp->flags[0]= (flag_t*)malloc(sizeof(flag_t)*2);
    tmp->flags[1]= (flag_t*)malloc(sizeof(flag_t)*1);
    tmp->flags[2]= NULL;

    tmp->flags[0][0]= 165;
    tmp->flags[0][1]= 170;

    tmp->flags[1][0]= 172;

    tmp->record_flags= inalloc(1);
    tmp->record_flags[0]= 5;

    tmp->primary_record_ids= NULL;

    return tmp;
}

/*int record_type;
int frequency;
int site;
int n_dates;
real* dates;
int n_time_of_observation;
char* time_of_observation;
int n_data;
temp_t* data;
int n_uncertainty;
real* uncertainty;
int n_num_measurements;
short* num_measurements;
int n_n_flags;
char* n_flags;
flag_t** flags;
int source;
int n_record_flags;
int* record_flags;
int n_primary_record_ids;
int* primary_record_ids;*/

void compressSE2(stationElement2p se)
{
    if ( se->compressed == true )
        return;

    unsigned int index;
    unsigned int size;
    index= 0;

    size= sizeof(real)*(se->n_dates);
    memcpy((void*)(compression_area + index), (void*)(se->dates), size);
    index+= size;

    size= sizeof(char)*(se->n_time_of_observation);
    memcpy((void*)(compression_area + index), (void*)(se->time_of_observation), size);
    index+= size;

    size= sizeof(temp_t)*(se->n_data);
    memcpy((void*)(compression_area + index), (void*)(se->data), size);
    index+= size;

    size= sizeof(real)*(se->n_uncertainty);
    memcpy((void*)(compression_area + index), (void*)(se->uncertainty), size);
    index+= size;

    size= sizeof(short)*(se->n_num_measurements);
    memcpy((void*)(compression_area + index), (void*)(se->num_measurements), size);
    index+= size;

    size= sizeof(char)*(se->n_n_flags);
    memcpy((void*)(compression_area + index), (void*)(se->n_flags), size);
    index+= size;

    int i;
    for ( i= 0; i < se->n_n_flags; ++i )
    {
        size= sizeof(flag_t)*(se->n_flags[i]);
        memcpy((void*)(compression_area + index), (void*)(se->flags[i]), size);
        index+= size;
    }

    size= sizeof(int)*(se->n_record_flags);
    memcpy((void*)(compression_area + index), (void*)(se->record_flags), size);
    index+= size;

    size= sizeof(int)*(se->n_primary_record_ids);
    memcpy((void*)(compression_area + index), (void*)(se->primary_record_ids), size);
    index+= size;

    se->dataLength= index;
    if ( se->compressedRecord= NULL )
        se->compressedRecord= (char*)malloc(sizeof(char)*index);
    else
        se->compressedRecord= (char*)realloc(se->compressedRecord, sizeof(char)*index);

    lzfx_compress((void*)compression_area, index, (void*)(se->compressedRecord), &(se->compressedRecordLength));
    se->compressedRecord= realloc(se->compressedRecord, se->compressedRecordLength);
    se->compressed= true;

    free(se->dates);
    free(se->time_of_observation);
    free(se->data);
    free(se->uncertainty);
    free(se->num_measurements);
    free(se->n_flags);
    for ( i= 0; i < se->n_n_flags; ++i )
        free(se->flags[i]);
    free(se->flags);
    free(se->record_flags);
    free(se->primary_record_ids);

    se->dates= NULL;
    se->time_of_observation= NULL;
    se->data= NULL;
    se->uncertainty= NULL;
    se->num_measurements= NULL;
    se->n_flags= NULL;
    se->flags= NULL;
    se->record_flags= NULL;
    se->primary_record_ids= NULL;
}

void decompressSE2(stationElement2p se)
{
    if ( se->compressed == false )
        return;

    unsigned int length= COMPRESSION_AREA;
    lzfx_decompress((void*)(se->compressedRecord), se->compressedRecordLength, compression_area, &length);

    unsigned int index= 0;
    unsigned int size;

    se->dates= (real*)rnalloc(se->n_dates);
    size= sizeof(real)*(se->n_dates);
    memcpy((void*)(se->dates), (void*)(compression_area + index), size);
    index+= size;

    se->time_of_observation= (char*)cnalloc(se->n_time_of_observation);
    size= sizeof(char)*(se->n_time_of_observation);
    memcpy((void*)(se->time_of_observation), (void*)(compression_area + index), size);
    index+= size;

    se->data= (temp_t*)tnalloc(se->n_data);
    size= sizeof(temp_t)*(se->n_data);
    memcpy((void*)(se->data), (void*)(compression_area + index), size);
    index+= size;

    se->uncertainty= (real*)rnalloc(se->n_uncertainty);
    size= sizeof(real)*(se->n_uncertainty);
    memcpy((void*)(se->uncertainty), (void*)(compression_area + index), size);
    index+= size;

    se->num_measurements= (short*)snalloc(se->n_num_measurements);
    size= sizeof(short)*(se->n_num_measurements);
    memcpy((void*)(se->num_measurements), (void*)(compression_area + index), size);
    index+= size;

    se->n_flags= (char*)cnalloc(se->n_n_flags);
    size= sizeof(char)*(se->n_n_flags);
    memcpy((void*)(se->n_flags), (void*)(compression_area + index), size);
    index+= size;

    se->flags= (flag_t**)malloc(sizeof(flag_t*)*(se->n_n_flags));
    int i;
    for ( i= 0; i < se->n_n_flags; ++i )
    {
        size= sizeof(flag_t)*(se->n_flags[i]);
        se->flags[i]= (flag_t*)malloc(size);
        memcpy((void*)(se->flags[i]), (void*)(compression_area + index), size);
        index+= size;
    }

    se->record_flags= (int*)inalloc(se->n_record_flags);
    size= sizeof(int)*(se->n_record_flags);
    memcpy((void*)(se->record_flags), (void*)(compression_area + index), size);
    index+= size;

    se->primary_record_ids= (int*)inalloc(se->n_primary_record_ids);
    size= sizeof(int)*(se->n_primary_record_ids);
    memcpy((void*)(se->primary_record_ids), (void*)(compression_area + index), size);
    index+= size;

    se->compressed= false;
}

unsigned int sizeSE2(stationElement2p se)
{
    int sum= sizeOfCompressableSE2(se);
    sum+= sizeof(int)*11;
    return sum;
}

unsigned int sizeOfCompressableSE2(stationElement2p se)
{
    int sum= 0;
    sum+= sizeof(real)*(se->n_dates);
    sum+= sizeof(char)*(se->n_time_of_observation);
    sum+= sizeof(temp_t)*(se->n_data);
    sum+= sizeof(real)*(se->n_uncertainty);
    sum+= sizeof(short)*(se->n_num_measurements);
    sum+= sizeof(int)*(se->n_record_flags);
    sum+= sizeof(int)*(se->n_primary_record_ids);
    sum+= sizeof(char*)*(se->n_n_flags);
    int i;
    int sum2= 0;
    for ( i= 0; i < se->n_n_flags; ++i )
        sum2+= se->n_flags[i];
    sum+= sizeof(flag_t)*sum2;

    return sum;
}

stationElement2* createSE2NC(stationElement2* se)
{
    //dea();
    stationElement2* tmp= (stationElement2*)calloc(1,sizeof(stationElement2));
    tmp->record_type= se->record_type;
    tmp->frequency= se->frequency;
    tmp->site= se->site;
    tmp->n_dates= se->n_dates;
    tmp->n_time_of_observation= se->n_time_of_observation;
    tmp->n_data= se->n_data;
    tmp->n_uncertainty= se->n_uncertainty;
    tmp->n_num_measurements= se->n_num_measurements;
    tmp->n_n_flags= se->n_n_flags;
    tmp->source= se->source;
    tmp->n_record_flags= se->n_record_flags;
    tmp->n_primary_record_ids= se->n_primary_record_ids;
    //deb();

    tmp->dates= (real*)copyRA(se->dates, se->n_dates);
    //dec();
    tmp->time_of_observation= (char*)copyCA(se->time_of_observation, se->n_time_of_observation);
    //ded();
    tmp->data= (temp_t*)copyTA(se->data, se->n_data);
    //dee();
    tmp->uncertainty= (real*)copyRA(se->uncertainty, se->n_uncertainty);
    //def();
    tmp->num_measurements= (short*)copySA(se->num_measurements, se->n_num_measurements);
    //deg();
    tmp->n_flags= (char*)copyCA(se->n_flags, se->n_n_flags);
    //deh();
    tmp->flags= (flag_t**)malloc(sizeof(flag_t*)*(se->n_n_flags));
    int i;
    for ( i= 0; i < se->n_n_flags; ++i )
        tmp->flags[i]= (flag_t*)copyFA(se->flags[i], se->n_flags[i]);
    //dei();
    tmp->record_flags= (int*)copyIA(se->record_flags, se->n_record_flags);
    //dej();
    tmp->primary_record_ids= (int*)copyIA(se->primary_record_ids, se->n_primary_record_ids);

    return tmp;
}

void destroySE2(stationElement2* se)
{
    dea();
    if ( !se )
        return;

    deb();
    if ( se->dates )
        free(se->dates);

    dec();
    if ( se->time_of_observation )
        free(se->time_of_observation);

    ded();
    if ( se->data )
        free(se->data);

    dee();
    if ( se->uncertainty )
        free(se->uncertainty);

    def();
    if ( se->num_measurements )
        free(se->num_measurements);

    deg();
    if ( se->n_flags )
        free(se->n_flags);

    deh();
    int i;
    for ( i= 0; i < se->n_n_flags; ++i )
        if ( se->flags[i] )
            free(se->flags[i]);

    dei();
    if ( se->flags )
        free(se->flags);

    dej();
    if ( se->record_flags )
        free(se->record_flags);

    dek();
    if ( se->primary_record_ids )
        free(se->primary_record_ids);

    free(se);
}

void destroySE2V(stationElement2p* se, int n)
{
    if ( !se )
        return;
    int i;
    for ( i= 0; i < n; ++i )
    {
        printf("%d ", i);
        destroySE2(se[i]);
    }
    free(se);
}

stationElement2* makeSingleValued(stationElement2* se, int* bf, int n)
{
    if ( isSingleValued(se) )
        return se;

    stationElement2* st1, *st2;
    st1= structureMerge(se);
    st2= mergeCore(st1, bf, n, "merge_any");
    return st2;
}

bool isSingleValued(stationElement2* se)
{
    if ( se->n_dates <= 1 )
        return true;

    real* d= diffRN(se->dates, se->n_dates);

    if ( minR(d, se->n_dates-1) > 0 )
    {
        free(d);
        return true;
    }

    return false;
}

bool isMultiValued(stationElement2* se)
{
    return !isSingleValued(se);
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

stationElement2* mergeCore(stationElement2* se, int* bf, int n, char* action)
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
        return se;

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

    int ls= 1;//se->n_sources;

    int* source= inalloc(ld*c*ls);

    int i, j, k, stride;
    stride= c;
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
        flagged[i*stride + 0]= orig_flagged[i];
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

void findFlags(stationElement2* se, flag_t* badFlags, int n_badFlags, int* f, int* n_f)
{
    int i, j, k, l= 0;
    for ( i= 0; i < se->n_n_flags; ++i )
        for ( j= 0; j < se->n_flags[i]; ++j )
        {
            for ( k= 0; k < n_badFlags; ++k )
                if ( se->flags[i][j] == badFlags[k] )
                {
                    f[l++]= i;
                    break;
                }
            if ( k != n_badFlags )
                break;
        }
    *n_f= l;
}

stationElement2p createSE2Select(stationElement2p se, int* f, int n_f)
{
    stationElement2* tmp= createSE2N();

    tmp->record_flags= copyIA(se->record_flags, se->n_record_flags);
    tmp->n_record_flags= se->n_record_flags;

    tmp->primary_record_ids= copyIA(se->primary_record_ids, se->n_primary_record_ids);
    tmp->n_primary_record_ids= se->n_primary_record_ids;

    tmp->dates= rnalloc(n_f);
    tmp->time_of_observation= cnalloc(n_f);
    tmp->data= tnalloc(n_f);
    tmp->uncertainty= rnalloc(n_f);
    tmp->num_measurements= snalloc(n_f);
    tmp->n_flags= cnalloc(n_f);
    tmp->flags= (flag_t**)malloc(sizeof(flag_t*)*n_f);

    int a= 0, i;
    for ( i= 0; i < n_f; ++i )
    {
        tmp->dates[a]= se->dates[f[i]];
        tmp->time_of_observation[a]= se->time_of_observation[f[i]];
        tmp->data[a]= se->data[f[i]];
        tmp->uncertainty[a]= se->uncertainty[f[i]];
        tmp->num_measurements[a]= se->num_measurements[f[i]];
        tmp->n_flags[a]= se->n_flags[f[i]];
        tmp->flags[a]= copyFA(se->flags[f[i]], se->n_flags[f[i]]);
        ++a;
    }

    tmp->frequency= se->frequency;
    tmp->site= se->site;
    tmp->n_dates= n_f;
    tmp->n_time_of_observation= n_f;
    tmp->n_data= n_f;
    tmp->n_uncertainty= n_f;
    tmp->n_num_measurements= n_f;
    tmp->n_n_flags= n_f;
    tmp->source= se->source;

    return tmp;
}
