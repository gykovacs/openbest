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
