#include "openbest-ds/stationSite2.h"
#include "openbest-ds/primaryKeyTables.h"

void displaySS2(stationSite2* ss)
{
    printf("station information:\n");
    printf("\t id: %d\n", ss->id);
    printf("\t country: %d %s\n", ss->country, lookupKeysPKT(countryNamesPKT, ss->country)[0]);
    printf("\t state: %s\n", ss->state);
    printf("\t county: %s\n", ss->county);
    printf("\t "); displayGP2(ss->location);
    printf("\n\t timezone: %d\n", ss->timezone);
    printf("\t name: %s\n", ss->primary_name);
    printf("\t relocated flag: %d\n", ss->relocated);
    printf("\t possibly_relocated: %d\n", ss->possible_relocated);
    printf("\t sources: %d\n", ss->n_sources);
    printf("\t ");
    int i;
    for ( i= 0; i < ss->n_sources; ++i )
        printf("%d %s, ", ss->sources[i], lookupKeysPKT(stationSourceTypesPKT, ss->sources[i])[0]);
    printf("\n\t flags: \n");
    printf("\t num of flags: %d\t", ss->n_flags);
    for ( i= 0; i < ss->n_flags; ++i )
    {
        printf("%d ", ss->flags[i]); fflush(stdout);
        printf("%s, ", lookupKeysPKT(siteFlagsPKT, ss->flags[i])[0]); fflush(stdout);
    }
    printf("\n");
}
