#include <string.h>
#include <stdlib.h>

#include "openbest-ds/stationElement2.h"
#include "openbest-ds/primaryKeyTables.h"
#include "openbest-ds/memFunctions.h"
#include "openbest-ds/mathFunctions.h"
#include "openbest-ds/basicAlgorithms.h"

void displaySE2(stationElement2* se)
{
    int i,j;

    printf("stationElement2:\n");
    printf("\t record_type: %d\n", se->record_type);
    printf("\t frequency: %d\n", se->frequency);
    printf("\t site: %d\n", se->site);
    printf("\t dates: %d - ", se->n_dates);

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
    /*printf("\n\t sources: %d: - ", se->n_sources);
    for ( i= 0; i < se->n_sources; ++i )
        printf("%d ", se->sources[i]);*/
    /*printf("\n\t relocations: %d - ", se->n_relocations);
    for ( i= 0; i < se->n_relocations; ++i )
        printf("%f,%d ", se->relocations[i], se->relocation_types[i]);*/

    printf("\n\t n_n_sources: %d - \n", se->n_n_sources);
    for ( i= 0; i < se->n_n_sources; ++i )
        for ( j= 0; j < se->n_sources[i]; ++j )
        {
            printf("%f ", se->dates[i]); fflush(stdout);
            printf("%d ", se->n_sources[i]); fflush(stdout);
            printf("%d ", se->sources[i][j]); fflush(stdout);
            printf("\n"); fflush(stdout);
        }
    printf("\n");
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
    tmp->n_sources= NULL;
    tmp->sources= NULL;
    tmp->record_flags= NULL;
    tmp->primary_record_ids= NULL;
    tmp->sources= NULL;
    /*tmp->relocations= NULL;
    tmp->relocation_types= NULL;*/
    tmp->record_type= -1;
    tmp->frequency= -1;
    tmp->site= -1;
    tmp->n_dates= 0;
    tmp->n_time_of_observation= 0;
    tmp->n_data= 0;
    tmp->n_uncertainty= 0;
    tmp->n_num_measurements= 0;
    tmp->n_n_flags= 0;
    tmp->n_n_sources= 0;
    tmp->n_record_flags= 0;
    tmp->n_primary_record_ids= 0;
    //tmp->n_relocations= 0;

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
    tmp->n_record_flags= 1;
    tmp->n_primary_record_ids= 0;
    tmp->n_n_sources= 3;
    //tmp->n_relocations= 2;

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

    tmp->n_sources= cnalloc(3);
    tmp->n_sources[0]= 2;
    tmp->n_sources[1]= 1;
    tmp->n_sources[2]= 0;

    tmp->sources= (flag_t**)malloc(sizeof(flag_t*)*3);
    tmp->sources[0]= (flag_t*)malloc(sizeof(flag_t)*2);
    tmp->sources[1]= (flag_t*)malloc(sizeof(flag_t)*1);
    tmp->sources[2]= NULL;

    tmp->sources[0][0]= 40;
    tmp->sources[0][1]= 20;

    tmp->sources[1][0]= 30;

    tmp->record_flags= inalloc(1);
    tmp->record_flags[0]= 5;

    tmp->primary_record_ids= NULL;

    /*tmp->sources= inalloc(2);
    tmp->sources[0]= 3;
    tmp->sources[1]= 10;*/

    /*tmp->relocations= rnalloc(2);
    tmp->relocations[0]= 1984.3;
    tmp->relocations[1]= 1984.4;

    tmp->relocation_types= fnalloc(2);
    tmp->relocation_types[0]= 1;
    tmp->relocation_types[1]= 2;*/

    return tmp;
}

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

    /*size= sizeof(int)*(se->n_sources);
    memcpy((void*)(compression_area + index), (void*)(se->sources), size);
    index+= size;*/

    /*size= sizeof(real)*(se->n_relocations);
    memcpy((void*)(compression_area + index), (void*)(se->relocations), size);
    index+= size;

    size= sizeof(flag_t)*(se->n_relocations);
    memcpy((void*)(compression_area + index), (void*)(se->relocation_types), size);
    index+= size;*/

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

    size= sizeof(char)*(se->n_n_sources);
    memcpy((void*)(compression_area + index), (void*)(se->n_sources), size);
    index+= size;

    for ( i= 0; i < se->n_n_sources; ++i )
    {
        size= sizeof(flag_t)*(se->n_sources[i]);
        memcpy((void*)(compression_area + index), (void*)(se->sources[i]), size);
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
    free(se->n_sources);
    for ( i= 0; i < se->n_n_sources; ++i )
        free(se->sources[i]);
    free(se->sources);
    free(se->record_flags);
    free(se->primary_record_ids);
    //free(se->sources);
    /*free(se->relocations);
    free(se->relocation_types);*/

    se->dates= NULL;
    se->time_of_observation= NULL;
    se->data= NULL;
    se->uncertainty= NULL;
    se->num_measurements= NULL;
    se->n_flags= NULL;
    se->flags= NULL;
    se->record_flags= NULL;
    se->primary_record_ids= NULL;
    se->sources= NULL;
    se->n_sources= NULL;
    /*se->relocations= NULL;
    se->relocation_types= NULL;*/
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
    //dea();
    se->num_measurements= (short*)snalloc(se->n_num_measurements);
    size= sizeof(short)*(se->n_num_measurements);
    memcpy((void*)(se->num_measurements), (void*)(compression_area + index), size);
    index+= size;
    //deb();
    /*se->sources= (int*)inalloc(se->n_sources);
    size= sizeof(int)*(se->n_sources);
    memcpy((void*)(se->sources), (void*)(compression_area + index), size);
    index+= size;*/

    /*se->relocations= (real*)rnalloc(se->n_relocations);
    size= sizeof(real)*(se->n_relocations);
    memcpy((void*)(se->relocations), (void*)(compression_area + index), size);
    index+= size;

    se->relocation_types= (flag_t*)fnalloc(se->n_relocations);
    size= sizeof(flag_t)*(se->n_relocations);
    memcpy((void*)(se->relocation_types), (void*)(compression_area + index), size);
    index+= size;*/
    //dec();
    se->n_flags= (char*)cnalloc(se->n_n_flags);
    size= sizeof(char)*(se->n_n_flags);
    memcpy((void*)(se->n_flags), (void*)(compression_area + index), size);
    index+= size;
    //ded();
    se->flags= (flag_t**)malloc(sizeof(flag_t*)*(se->n_n_flags));
    int i;
    for ( i= 0; i < se->n_n_flags; ++i )
    {
        size= sizeof(flag_t)*(se->n_flags[i]);
        se->flags[i]= (flag_t*)malloc(size);
        memcpy((void*)(se->flags[i]), (void*)(compression_area + index), size);
        index+= size;
    }
    //dee();
    se->n_sources= (char*)cnalloc(se->n_n_sources);
    size= sizeof(char)*(se->n_n_sources);
    memcpy((void*)(se->n_sources), (void*)(compression_area + index), size);
    index+= size;
    //def();
    se->sources= (flag_t**)malloc(sizeof(flag_t*)*(se->n_n_sources));
    for ( i= 0; i < se->n_n_sources; ++i )
    {
        size= sizeof(flag_t)*(se->n_sources[i]);
        se->sources[i]= (flag_t*)malloc(size);
        memcpy((void*)(se->sources[i]), (void*)(compression_area + index), size);
        index+= size;
    }
    //deg();
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
    /*sum+= sizeof(real)*(se->n_relocations);
    sum+= sizeof(flag_t)*(se->n_relocations);*/
    sum+= sizeof(char*)*(se->n_n_flags);
    int i;
    int sum2= 0;
    for ( i= 0; i < se->n_n_flags; ++i )
        sum2+= se->n_flags[i];
    sum+= sizeof(flag_t)*sum2;
    sum+= sizeof(char*)*(se->n_n_sources);
    sum2= 0;
    for ( i= 0; i < se->n_n_sources; ++i )
        sum2+= se->n_sources[i];
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
    tmp->n_record_flags= se->n_record_flags;
    tmp->n_primary_record_ids= se->n_primary_record_ids;
    tmp->n_n_sources= se->n_n_sources;
    //tmp->n_relocations= se->n_relocations;
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
    tmp->n_sources= (char*)copyCA(se->n_sources, se->n_n_sources);

    tmp->sources= (flag_t**)malloc(sizeof(flag_t*)*(se->n_n_sources));
    for ( i= 0; i < se->n_n_sources; ++i )
        tmp->sources[i]= (flag_t*)copyFA(se->sources[i], se->n_sources[i]);

    tmp->record_flags= (int*)copyIA(se->record_flags, se->n_record_flags);
    //dej();
    tmp->primary_record_ids= (int*)copyIA(se->primary_record_ids, se->n_primary_record_ids);

    //tmp->n_sources= (int*)copyIA(se->sources, se->n_sources);

    /*tmp->relocations= (real*)copyRA(se->relocations, se->n_relocations);

    tmp->relocation_types= (flag_t*)copyFA(se->relocation_types, se->n_relocations);*/

    return tmp;
}

void destroySE2(stationElement2* se)
{
    //dea();
    if ( !se )
        return;

    //deb();
    if ( se->dates )
        free(se->dates);

    //dec();
    if ( se->time_of_observation )
        free(se->time_of_observation);

    //ded();
    if ( se->data )
        free(se->data);

    //dee();
    if ( se->uncertainty )
        free(se->uncertainty);

    //def();
    if ( se->num_measurements )
        free(se->num_measurements);

    //deg();
    if ( se->n_flags )
        free(se->n_flags);

    //deh();
    int i;
    for ( i= 0; i < se->n_n_flags; ++i )
        if ( se->flags[i] )
            free(se->flags[i]);

    //dei();
    if ( se->flags )
        free(se->flags);

    if ( se->n_sources )
        free(se->n_sources);

    for ( i= 0; i < se->n_n_sources; ++i )
        if ( se->sources[i] )
            free(se->sources[i]);

    if ( se->sources )
        free(se->sources);

    //dej();
    if ( se->record_flags )
        free(se->record_flags);

    //dek();
    if ( se->primary_record_ids )
        free(se->primary_record_ids);

    /*if ( se->sources )
        free(se->sources);*/

    /*if ( se->relocations )
        free(se->relocations);

    if ( se->relocation_types )
        free(se->relocation_types);*/

    free(se);
}

void destroySE2V(stationElement2p* se, int n)
{
    if ( !se )
        return;
    int i;
    for ( i= 0; i < n; ++i )
    {
        //printf("%d ", i);
        destroySE2(se[i]);
    }
    free(se);
}



bool isSingleValued(stationElement2* se)
{
    if ( se->n_dates <= 1 )
        return true;

    int i;
    printf("%d\n", se->n_dates);
    for ( i= 0; i < se->n_dates; ++i )
        printf("%f ", se->dates[i]);
    printf("\n");
    real* d= diffRN(se->dates, se->n_dates);
    for ( i= 0; i < se->n_dates-1; ++i )
        printf("%f ", d[i]);
    printf("\n");


    printf("%f\n", minR(d, se->n_dates - 1));

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



void findFlags(stationElement2* se, int* badFlags, int n_badFlags, int* f, int* n_f)
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

    /*tmp->relocations= rnalloc(se->n_relocations);
    tmp->relocation_types= fnalloc(se->n_relocations);*/
    tmp->n_flags= cnalloc(n_f);
    tmp->flags= (flag_t**)malloc(sizeof(flag_t*)*n_f);

    tmp->n_sources= cnalloc(n_f);
    tmp->sources= (flag_t**)malloc(sizeof(flag_t*)*n_f);

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
        tmp->n_sources[a]= se->n_sources[f[i]];
        tmp->sources[a]= copyFA(se->sources[f[i]], se->n_sources[f[i]]);
        ++a;
    }

    /*for ( i= 0; i < se->n_sources; ++i )
        tmp->sources[i]= se->sources[i];*/

    /*for ( i= 0; i < se->n_relocations; ++i )
    {
        tmp->relocations[i]= se->relocations[i];
        tmp->relocation_types= se->relocation_types[i];
    }*/

    tmp->frequency= se->frequency;
    tmp->site= se->site;
    tmp->n_dates= n_f;
    tmp->n_time_of_observation= n_f;
    tmp->n_data= n_f;
    tmp->n_uncertainty= n_f;
    tmp->n_num_measurements= n_f;
    tmp->n_n_flags= n_f;
    tmp->n_n_sources= n_f;
    //tmp->n_relocations= se->n_relocations;

    return tmp;
}

int longestDataSeries(stationElement2p* se, int n)
{
    int res= 0;
    int i;
    for ( i= 0; i < n; ++i )
        if ( res < se[i]->n_dates )
            res= se[i]->n_dates;
    return res;
}

void sortSE2DataByDate(stationElement2p se)
{
    int* tmp= inalloc(se->n_data);
    outer_array= se->dates;
    int i;
    for ( i= 0; i < se->n_data; ++i )
        tmp[i]= i;

    qsortORIA(tmp, se->n_dates);

    stationElement2p seTmp= createSE2NC(se);
    for ( i= 0; i < se->n_data; ++i )
    {
        free(se->flags[i]);
        free(se->sources[i]);
    }
    for ( i= 0; i < se->n_data; ++i )
    {
        se->data[i]= seTmp->data[tmp[i]];
        se->dates[i]= seTmp->dates[tmp[i]];
        se->time_of_observation[i]= seTmp->time_of_observation[tmp[i]];
        se->uncertainty[i]= seTmp->uncertainty[tmp[i]];
        se->num_measurements[i]= seTmp->num_measurements[tmp[i]];
        se->n_flags[i]= seTmp->n_flags[tmp[i]];
        se->n_sources[i]= seTmp->n_sources[tmp[i]];
        se->flags[i]= seTmp->flags[tmp[i]];
        seTmp->flags[tmp[i]]= NULL;
        se->sources[i]= seTmp->sources[tmp[i]];
        seTmp->sources[tmp[i]]= NULL;
    }

    free(tmp);
    if ( seTmp->data )
        free(seTmp->data);
    if ( seTmp->dates )
        free(seTmp->dates);
    if ( seTmp->time_of_observation )
        free(seTmp->time_of_observation);
    if ( seTmp->uncertainty)
        free(seTmp->uncertainty);
    if ( seTmp->num_measurements)
        free(seTmp->num_measurements);
    if ( seTmp->n_flags )
        free(seTmp->n_flags);
    if ( seTmp->n_sources )
        free(seTmp->n_sources);
    if ( seTmp->flags)
        free(seTmp->flags);
    if ( seTmp->sources )
        free(seTmp->sources);
    if ( seTmp->record_flags )
        free(seTmp->record_flags);
    if ( seTmp->primary_record_ids )
        free(seTmp->primary_record_ids);
    free(seTmp);
}
