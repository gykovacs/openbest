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

void destroySE2(stationElement2* se)
{
    if ( se->dates )
        free(se->dates);

    if ( se->time_of_observation )
        free(se->time_of_observation);

    if ( se->data )
        free(se->data);

    if ( se->uncertainty )
        free(se->uncertainty);

    if ( se->num_measurements )
        free(se->num_measurements);

    if ( se->n_flags )
        free(se->n_flags);

    int i;
    for ( i= 0; i < se->n_n_flags; ++i )
        if ( se->flags[i] )
            free(se->flags[i]);

    if ( se->flags )
        free(se->flags);

    if ( se->record_flags )
        free(se->record_flags);

    if ( se->primary_record_ids )
        free(se->primary_record_ids);

    free(se);
}

void destroySE2V(stationElement2p* se, int n)
{
    int i;
    for ( i= 0; i < n; ++i )
        destroySE2(se[i]);
    free(se);
}
