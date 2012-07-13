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

int id;
int country;
char state[100];
char county[100];
geoPoint2* location;
geoPoint2* all_locations;
timeInstant* all_location_times;
int timezone;
char primary_name[50];
char** alt_names;
int relocated;
int possible_relocated;
int* instrument_changes;
int* primary_manifests;
int* secondary_manifests;
int n_flags;
int* flags;
int n_sources;
int* sources;
int* archive_keys;

stationSite2* createSS2N()
{
    stationSite2* tmp= (stationSite2*)malloc(sizeof(stationSite2));
    tmp->location= NULL;
    tmp->all_locations= NULL;
    tmp->all_location_times= NULL;
    tmp->alt_names= NULL;
    tmp->instrument_changes= NULL;
    tmp->primary_manifests= NULL;
    tmp->secondary_manifests= NULL;
    tmp->flags= NULL;
    tmp->sources= NULL;
    tmp->archive_keys= NULL;
    tmp->id= -1;
    tmp->country= -1;
    tmp->timezone= -1;
    tmp->relocated= -1;
    tmp->possible_relocated= -1;
    tmp->n_flags= 0;
    tmp->n_sources= 0;
    return tmp;
}

void destroySS2(stationSite2* ss)
{
    if ( ss->location)
        free(ss->location);
    if ( ss->all_locations)
        free(ss->all_locations);
    if ( ss->all_location_times )
        free(ss->all_location_times);
    if ( ss->alt_names )
        free(ss->alt_names );
    if ( ss->instrument_changes )
        free(ss->instrument_changes );
    if ( ss->primary_manifests )
        free(ss->primary_manifests);
    if ( ss->secondary_manifests )
        free(ss->secondary_manifests);
    if ( ss->sources )
        free(ss->sources );
    if ( ss->archive_keys )
        free(ss->archive_keys );
    if ( ss->flags )
        free(ss->flags);
    free(ss);
}

void destroySS2V(stationSite2p* ss, int n)
{
    int i;
    for ( i= 0; i < n; ++i )
        destroySS2(ss[i]);
    free(ss);
}
