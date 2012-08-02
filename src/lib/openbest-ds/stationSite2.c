#include <stdlib.h>
#include <string.h>

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
    printf("\n\t relocations: %d - ", ss->n_relocations);
    for ( i= 0; i < ss->n_relocations; ++i )
        printf("%f,%d ", ss->relocations[i], ss->relocation_types[i]);
    printf("\n"); fflush(stdout);
}

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
    tmp->relocations= NULL;
    tmp->relocation_types= NULL;
    tmp->id= -1;
    tmp->country= -1;
    tmp->timezone= -1;
    tmp->relocated= -1;
    tmp->possible_relocated= -1;
    tmp->n_flags= 0;
    tmp->n_sources= 0;
    tmp->n_relocations= 0;

    return tmp;
}

stationSite2* createSS2NC(stationSite2* ss)
{
    stationSite2* tmp= createSS2N();
    tmp->id= ss->id;
    tmp->country= ss->country;
    strcpy(tmp->state, ss->state);
    strcpy(tmp->county, ss->county);
    tmp->location= createGeoPoint2C(ss->location);
    tmp->all_locations= ss->all_locations;
    tmp->all_location_times= ss->all_location_times;
    tmp->timezone= ss->timezone;
    strcpy(tmp->primary_name, ss->primary_name);
    tmp->alt_names= ss->alt_names;
    tmp->relocated= ss->relocated;
    tmp->possible_relocated= ss->possible_relocated;
    tmp->instrument_changes= ss->instrument_changes;
    tmp->primary_manifests= ss->primary_manifests;
    tmp->secondary_manifests= ss->secondary_manifests;
    tmp->n_flags= ss->n_flags;
    tmp->flags= inalloc(tmp->n_flags);
    memcpy(tmp->flags, ss->flags, sizeof(int)*(tmp->n_flags));
    tmp->n_sources= ss->n_sources;
    tmp->sources= inalloc(tmp->n_sources);
    memcpy(tmp->sources, ss->sources, sizeof(int)*(tmp->n_sources));
    tmp->archive_keys= ss->archive_keys;
    tmp->n_relocations= ss->n_relocations;
    tmp->relocations= rnalloc(tmp->n_relocations);
    memcpy(tmp->relocations, ss->relocations, sizeof(real)*(tmp->n_relocations));
    tmp->relocation_types= fnalloc(tmp->n_relocations);
    memcpy(tmp->relocation_types, ss->relocation_types, sizeof(flag_t)*(tmp->n_relocations));

    return tmp;
}

void destroySS2(stationSite2* ss)
{
    if ( ss == NULL )
        return;
    //printf("a"); fflush(stdout);
    if ( ss->location)
        free(ss->location);
    //printf("b"); fflush(stdout);
    if ( ss->all_locations)
        free(ss->all_locations);
    //printf("c"); fflush(stdout);
    if ( ss->all_location_times )
        free(ss->all_location_times);
    //printf("d"); fflush(stdout);
    if ( ss->alt_names )
        free(ss->alt_names );
    //printf("e"); fflush(stdout);
    if ( ss->instrument_changes )
        free(ss->instrument_changes );
    //printf("f"); fflush(stdout);
    if ( ss->primary_manifests )
        free(ss->primary_manifests);
    //printf("g"); fflush(stdout);
    if ( ss->secondary_manifests )
        free(ss->secondary_manifests);
    //printf("h"); fflush(stdout);
    if ( ss->sources )
        free(ss->sources );
    //printf("i"); fflush(stdout);
    if ( ss->archive_keys )
        free(ss->archive_keys );
    //printf("j"); fflush(stdout);
    if ( ss->flags )
        free(ss->flags);
    //printf("k"); fflush(stdout);
    if ( ss->relocations )
        free(ss->relocations);
    if ( ss->relocation_types )
        free(ss->relocation_types);
    free(ss);
}

void destroySS2V(stationSite2p* ss, int n)
{
    if ( ss )
    {
        int i, j;
        for ( i= 0; i < n; ++i )
            for ( j= i+1; j < n; ++j )
                if ( ss[i] == ss[j] )
                    ss[j]= NULL;

        /*for ( i= 0; i < n; ++i )
            if ( ss[i] )
                printf("1");
            else
                printf("0");
        for ( i= 0; i < n; ++i )
            printf("%p\n", ss[i]);*/
        for ( i= 0; i < n; ++i )
        {
            //printf("%d ", i);
            /*if ( i == 3202 || i == 3345 )
            {
                printf("%p\n", ss[i]); fflush(stdout);
                //displaySS2(ss[i]);
            printf("-------"); fflush(stdout);
            }*/

            destroySS2(ss[i]);
        }
        free(ss);
    }
}

void shrinkSS2V(stationSite2p** ss1, int* n)
{
    stationSite2p* ss= *ss1;
    int i, j;

    bool* flags= (bool*)malloc(sizeof(bool)*(*n));
    if ( !flags )
        eprintf("malloc failed in shrinkSS2V");

    /*for ( i= 0; i < *n; ++i )
        printf("%d ", ss[i]->id);
    printf("\n=================================\n");*/
    qsort(ss, *n, sizeof(stationSite2p), (int(*)(const void*, const void*))&sortByIDSS2V);

    /*for ( i= 0; i < *n; ++i )
        printf("%d ", ss[i]->id);*/

    for ( i= 0; i < *n; ++i )
        flags[i]= false;

    int m= 0;
    int nn= *n;
    int base;

    tprintf("start shrinking\n");
    for ( i= 0; i < *n - 1; ++i )
    {
        if ( i != *n - 2 && ss[i]->id == ss[i+1]->id )
            ++m;
        else if ( i == *n - 2 || ss[i]->id != ss[i+1]->id )
        {
            if ( m > 0 )
            {
                base= i-m;
                ss[base]->sources= (int*)realloc(ss[base]->sources, sizeof(int)*(m+1));
                if ( !ss[base]->sources )
                    eprintf("malloc failed in shrinkSS2V");
                for ( j= base+1; j <= i; ++j )
                {
                    ss[base]->sources[j-base]= ss[j]->sources[0];
                    flags[j]= true;
                    --nn;
                }
                /*for ( j= 0; j < m; ++j )
                    printf("%d ", ss[base]->sources[j]);*/
                //printf("\n");
                ss[base]->n_sources+= m;
                m= 0;
            }
        }
    }

    tprintf("shrinking finished %d\n", nn);

    stationSite2p* tmp= (stationSite2p*)malloc(sizeof(stationSite2p)*(nn));
    if ( !tmp )
        eprintf("malloc failed in shrinkSS2V");
    j= 0;
    for ( i= 0; i < *n; ++i )
        if ( flags[i] )
        {
            destroySS2(ss[i]);
        }
        else
            tmp[j++]= ss[i];

    free(ss);
    *ss1= tmp;
    *n= nn;

    free(flags);
}

int sortByIDSS2V(const void* a, const void* b)
{
    return (*(stationSite2p*)a)->id - (*(stationSite2p*)b)->id;
}
