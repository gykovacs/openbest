#include <string.h>
#include <stdlib.h>

#include "openbest-ds/stationElement2.h"
#include "openbest-ds/primaryKeyTables.h"

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
    printf("\n\t n_num_measurements: - %d\n", se->n_num_measurements);
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

    //dej();
    if ( se->record_flags )
        free(se->record_flags);

    //dek();
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
        destroySE2(se[i]);
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
    displaySE2(se);
    displaySE2(result_st);

    def();
    destroySE2(result_st);
    deh();


    return se;
}
