#include <string.h>

#include "openbest-ds/config-ds.h"
#include "openbest-io/loadData.h"
#include "openbest-ds/memFunctions.h"

#define MAX_LENGTH_OF_TIME_SERIES 40000
#define MAX_DATA_FLAGS 30
#define MAX_SOURCES 10

#define MAX_SITE_FLAGS 11
#define MAX_STATION_CHANGES 1000

void loadPreliminaryData()
{
    loadCountryCodesPKT();
    loadCountryNamesPKT();
    loadStationRecordTypesPKT();
    loadStationSourceTypesPKT();
    loadDataFlagsPKT();
    loadSiteFlagsPKT();
    loadMatchFlagsPKT();
    loadOtherFlags();
}

void trimlr(char* a, char* b)
{
    int i= 0, j= strlen(a)-1, k;
    while ( a[i] == ' ' || a[i] == '\t' )
        ++i;
    while ( a[j] == ' ' || a[j] == '\t' || a[j] == '\n' )
        --j;
    //printf("trim: %d %d\n", i, j); fflush(stdout);
    for ( k= i; k <= j; ++k )
        b[k-i]= a[k];
    b[k]= '\0';
}

void loadStationSite2(stationSite2p** ss, int* n_stationSite2)
{
    /* READING STATION DATA */

    char filename[100];
    //strcpy(filename, DATA_DIR );
    strcpy(filename, SITE_DATA );
    FILE* f= fopen(filename, "rt");

    char buffer[1000];
    char tmpstr[100];

    printf("%s\n", filename);
    char* tmp;
    char delims[]= "\t";

    stationSite2p* p= (stationSite2p*)malloc(sizeof(stationSite2p)*10000);
    int capacity= 10000, n= 0;

    float latitude, longitude, elevation, lat_unc, long_unc, elev_unc;
    int id;

    while ( fgets(buffer, 1000, f) != NULL )
    {
      if ( buffer[0] == '%' )
        continue;

      if ( n == capacity )
      {
          p= (stationSite2p*)realloc(p, sizeof(stationSite2p)*(n + 10000));
          capacity+= 10000;
      }
      p[n]= (stationSite2p)createSS2N();

      //printf("%s\n", buffer);

      //printf("station id\n"); fflush(stdout);
      //stationID
      tmp= strtok(buffer, delims);
      sscanf(tmp, "%d", &id);
      //fprintf(stderr, "%d ", id);
      //printf("%d\n", id);
      p[n]->id= id;

      //report number
      tmp= strtok(NULL, delims);

      //station name
      tmp= strtok(NULL, delims);
      //printf("%s\n", tmp);
      strcpy(p[n]->primary_name, tmp);

      //country
      tmp= strtok(NULL, delims);

      trimlr(tmp, tmpstr);
      //printf("%s\n", tmpstr);
      p[n]->country= lookupValuePKT(countryNamesPKT, tmpstr);
      //printf("%d\n", p[n]->country);

      //record start date
      tmp= strtok(NULL, delims);

      //record end date
      tmp= strtok(NULL, delims);

      //source
      tmp= strtok(NULL, delims);
      p[n]->sources= (int*)malloc(sizeof(int)*1);
      p[n]->n_sources= 1;
      trimlr(tmp, tmpstr);
      p[n]->sources[0]= lookupValuePKT(stationSourceTypesPKT, tmpstr);
      //printf("%d\n", p[n]->sources[0]);

      //latitude
      tmp= strtok(NULL, delims);
      sscanf(tmp, "%f", &latitude);

      //longitude
      tmp= strtok(NULL, delims);
      sscanf(tmp, "%f", &longitude);

      //elevation
      tmp= strtok(NULL, delims);
      sscanf(tmp, "%f", &elevation);

      //alternative elevation
      tmp= strtok(NULL, delims);

      //lat unc.
      tmp= strtok(NULL, delims);
      sscanf(tmp, "%f", &lat_unc);

      //long unc.
      tmp= strtok(NULL, delims);
      sscanf(tmp, "%f", &long_unc);

      //elev unc.
      tmp= strtok(NULL, delims);
      sscanf(tmp, "%f", &elev_unc);

      //printf("%f, %f, %f\n", latitude, longitude, elevation);
      p[n]->location= createGeoPoint26(latitude, longitude, elevation, lat_unc, long_unc, elev_unc);

      //displayGP2(p[n]->location);

      //relocation flag
      tmp= strtok(NULL, delims);
      sscanf(tmp, "%d", &(p[n]->relocated));
      //printf("%d\n", p[n]->relocated);

      //state
      tmp= strtok(NULL, delims);
      strcpy(p[n]->state, tmp);
      //printf("%s\n", p[n]->state);

      //county
      tmp= strtok(NULL, delims);
      strcpy(p[n]->county, tmp);
      //printf("%s\n", p[n]->county);

      //timezone
      tmp= strtok(NULL, delims);
      sscanf(tmp, "%d", &id);
      p[n]->timezone= id;
      //printf("%d\n", p[n]->timezone);

      ++n;
    }

    p= (stationSite2**)realloc(p, sizeof(stationSite2*)*n);

    *ss= p;
    *n_stationSite2= n;

    printf("%d station site descriptors read in\n", n);
    fclose(f);

    /* READING FLAGS */

    //strcpy(filename, DATA_DIR );
    strcpy(filename, SITE_FLAGS );

    f= fopen(filename, "rt");
    int flags[11];
    int nf;
    int i;
    int trash;
    int* pflags;
    int nth= 0;
    while ( !feof(f) )
    {
        nf= 0;
        fscanf(f, "%d", &id);
        do
        {
            fscanf(f, "%d", &(flags[nf++]));
            if ( flags[nf-1] == 0 || nf == 11 )
                break;
        }while ( 1 );
        for ( i= nf; i < 11; ++i )
            fscanf(f, "%d", &trash);

        if ( flags[nf-1] == 0 )
            --nf;


        while ( nth < n && p[nth]->id <= id )
        {
            if ( p[nth]->id == id )
            {
                pflags= (int*)malloc(sizeof(int)*nf);
                if ( pflags == NULL )
                    eprintf("malloc failed in loadStationSite2");
                for ( i= 0; i < nf; ++i )
                    pflags[i]= flags[i];
                p[nth]->flags= pflags;
                p[nth]->n_flags= nf;
            }
            ++nth;
        }
    }
    tprintf("reading flags finished\n");
    fclose(f);

    /* READING SOURCES */

    strcpy(filename, STATION_CHANGE );
    tprintf("opening file: %s\n", filename);
    f= fopen(filename, "rt");

    nth= 0;
    real changes[MAX_STATION_CHANGES];
    flag_t types[MAX_STATION_CHANGES];
    int last_id= -1;
    int series, last_series= -1;
    id= -1;
    float rtmp;
    int ftmp;
    tprintf("reading site changes\n");
    nf= 0;
    int cnt= 0;
    int nf2;
    while ( fgets(buffer, 1000, f) != NULL )
    {
        last_id= id;

        if ( buffer[0] == '%' )
            continue;
        tmp= strtok(buffer, delims);
        sscanf(tmp, "%d", &id);

        // change event number
        last_series= series;
        tmp= strtok(NULL, delims);
        sscanf(tmp, "%d", &series);

        if ( (id != last_id) && last_id != -1 )
        {
            nf2= 1;
            for ( i= 1; i < nf; ++i )
            {
                if ( changes[nf2-1] < changes[i] )
                {
                    changes[nf2]= changes[i];
                    types[nf2]= types[i];
                    ++nf2;
                }
            }
            nf= nf2;
            while ( p[cnt]->id <= last_id )
            {
                if ( p[cnt]->id == last_id )
                {
                    p[cnt]->n_relocations= nf;
                    p[cnt]->relocations= rnalloc(nf);
                    p[cnt]->relocation_types= fnalloc(nf);
                    memcpy(p[cnt]->relocations, changes, sizeof(real)*nf);
                    memcpy(p[cnt]->relocation_types, types, sizeof(flag_t)*nf);
                }
                ++cnt;
            }

            nf= 0;
        }

        tmp= strtok(NULL, delims);
        sscanf(tmp, "%f", &rtmp);
        changes[nf]= rtmp;

        tmp= strtok(NULL, delims);
        sscanf(tmp, "%d", &ftmp);
        types[nf]= ftmp;

        nf++;
    }

    {
        p[last_id]->n_relocations= nf;
        p[last_id]->relocations= rnalloc(nf);
        p[last_id]->relocation_types= fnalloc(nf);
        memcpy(p[last_id]->relocations, changes, sizeof(real)*nf);
        memcpy(p[last_id]->relocation_types, types, sizeof(flag_t)*nf);
    }

    fclose(f);
    tprintf("reading of site changes finished\n");

}


void loadStationElement2(stationElement2p** se, int* n_stationElement2)
{
    tprintf("reading station elements\n");

    char filename[100];
    strcpy(filename, TEMP_DATA );
    FILE* f= fopen(filename, "rt");

    char buffer[1000];

    printf("%s\n", filename);

    char* tmp;
    char delims[]= "\t";

    stationElement2p* p= (stationElement2p*)malloc(sizeof(stationElement2p)*10000);
    int capacity= 10000, n= -1;

    int id;
    real datesp[MAX_LENGTH_OF_TIME_SERIES];
    temp_t tempp[MAX_LENGTH_OF_TIME_SERIES];
    real uncp[MAX_LENGTH_OF_TIME_SERIES];
    short obsp[MAX_LENGTH_OF_TIME_SERIES];
    char tobp[MAX_LENGTH_OF_TIME_SERIES];
    int tsn;
    int last_id= -1;
    float rtmp;

    while ( fgets(buffer, 1000, f) != NULL )
    {
      if ( buffer[0] == '%' )
        continue;

      if ( n == capacity - 1 )
      {
          printf("increase capacity...\n"); fflush(stdout);
          p= (stationElement2p*)realloc(p, sizeof(stationElement2p)*(n + 10000));
          capacity+= 10000;
      }

      //stationID
      tmp= strtok(buffer, delims);
      sscanf(tmp, "%d", &id);

      //dea();

      if ( last_id != id )
      {
          if ( last_id != -1 )
          {
              p[n]->dates= rnalloc(tsn);
              if ( p[n]->dates == NULL ) eprintf("realloc failed in loadData");
              p[n]->time_of_observation= cnalloc(tsn);
              if ( p[n]->time_of_observation == NULL ) eprintf("realloc failed in loadData");
              p[n]->data= tnalloc(tsn);
              if ( p[n]->data == NULL ) eprintf("realloc failed in loadData");
              p[n]->uncertainty= rnalloc(tsn);
              if ( p[n]->uncertainty == NULL ) eprintf("realloc failed in loadData");
              p[n]->num_measurements= snalloc(tsn);
              if ( p[n]->num_measurements == NULL ) eprintf("realloc failed in loadData");
              memcpy(p[n]->dates, datesp, sizeof(real)*tsn);
              memcpy(p[n]->time_of_observation, tobp, sizeof(char)*tsn);
              memcpy(p[n]->data, tempp, sizeof(temp_t)*tsn);
              memcpy(p[n]->uncertainty, uncp, sizeof(real)*tsn);
              memcpy(p[n]->num_measurements, obsp, sizeof(short)*tsn);
              p[n]->n_flags= (char*)malloc(sizeof(char)*tsn);
              if ( p[n]->n_flags == NULL ) eprintf("malloc failed in loadData");
              p[n]->flags= (flag_t**)malloc(sizeof(flag_t*)*tsn);
              if ( p[n]->flags == NULL ) eprintf("malloc failed in loadData");
              p[n]->n_sources= (char*)malloc(sizeof(char)*tsn);
              if ( p[n]->n_sources == NULL ) eprintf("malloc failed in loadData");
              p[n]->sources= (flag_t**)malloc(sizeof(flag_t*)*tsn);
              if ( p[n]->sources == NULL ) eprintf("malloc failed in loadData");
              p[n]->n_dates= p[n]->n_time_of_observation= p[n]->n_data= p[n]->n_num_measurements= p[n]->n_uncertainty= p[n]->n_n_flags= p[n]->n_n_sources= tsn;
          }

          ++n;
          p[n]= (stationElement2*)createSE2N();
          tsn= 0;
          p[n]->site= id;
      }

      //series number
      tmp= strtok(NULL, delims);

      //date
      tmp= strtok(NULL, delims);
      sscanf(tmp, "%f", &rtmp);
      datesp[tsn]= rtmp;

      //temperature
      tmp= strtok(NULL, delims);
      sscanf(tmp, "%f", &rtmp);
      tempp[tsn]= rtmp;

      //uncertainty
      tmp= strtok(NULL, delims);
      sscanf(tmp, "%f", &rtmp);
      uncp[tsn]= rtmp;

      //observations
      int s;
      tmp= strtok(NULL, delims);
      sscanf(tmp, "%d", &s);
      (obsp[tsn])= s;

      int t;
      //time of observation
      tmp= strtok(NULL, delims);
      sscanf(tmp, "%d", &t);
      (tobp[tsn])= t;

      ++tsn;
      last_id= id;
    }

    // last station element
    {
        p[n]->dates= rnalloc(tsn);
        if ( p[n]->dates == NULL ) eprintf("realloc failed in loadData");
        p[n]->time_of_observation= cnalloc(tsn);
        if ( p[n]->time_of_observation == NULL ) eprintf("realloc failed in loadData");
        p[n]->data= tnalloc(tsn);
        if ( p[n]->data == NULL ) eprintf("realloc failed in loadData");
        p[n]->uncertainty= rnalloc(tsn);
        if ( p[n]->uncertainty == NULL ) eprintf("realloc failed in loadData");
        p[n]->num_measurements= snalloc(tsn);
        if ( p[n]->num_measurements == NULL ) eprintf("realloc failed in loadData");
        memcpy(p[n]->dates, datesp, sizeof(real)*tsn);
        memcpy(p[n]->time_of_observation, tobp, sizeof(char)*tsn);
        memcpy(p[n]->data, tempp, sizeof(temp_t)*tsn);
        memcpy(p[n]->uncertainty, uncp, sizeof(real)*tsn);
        memcpy(p[n]->num_measurements, obsp, sizeof(short)*tsn);
        p[n]->n_flags= (char*)malloc(sizeof(char)*tsn);
        if ( p[n]->n_flags == NULL ) eprintf("malloc failed in loadData");
        p[n]->flags= (flag_t**)malloc(sizeof(flag_t*)*tsn);
        if ( p[n]->flags == NULL ) eprintf("malloc failed in loadData");
        p[n]->n_sources= (char*)malloc(sizeof(char)*tsn);
        if ( p[n]->n_sources == NULL ) eprintf("malloc failed in loadData");
        p[n]->sources= (flag_t**)malloc(sizeof(flag_t*)*tsn);
        if ( p[n]->sources == NULL ) eprintf("malloc failed in loadData");
        p[n]->n_dates= p[n]->n_time_of_observation= p[n]->n_data= p[n]->n_num_measurements= p[n]->n_uncertainty= p[n]->n_n_flags= p[n]->n_n_sources= tsn;
    }

    *se= p;
    *n_stationElement2= n + 1;

    fclose(f);

    tprintf("read station element data finished\n");

    // READING FLAGS
    tprintf("reading station flags\n");

    strcpy(filename, TEMP_FLAGS );
    f= fopen(filename, "rt");
    printf("%s\n", filename);

    last_id= -1;
    int flag;
    int i, j;
    n= -1;
    while ( fgets(buffer, 1000, f) != NULL )
    {
      if ( buffer[0] == '%' )
        continue;

      //station id
      tmp= strtok(buffer, delims);
      sscanf(tmp, "%d", &id);

      if ( last_id != id )
      {
          if ( last_id != -1 )
          {
              for ( i= 0; i < p[n]->n_n_flags; ++i )
              {
                  p[n]->flags[i]= realloc(p[n]->flags[i], sizeof(flag_t)*(p[n]->n_flags[i]));
                  if ( p[n]->flags[i] == NULL ) eprintf("realloc failed in loadData");
              }
          }

          ++n;
          if ( n == *n_stationElement2 )
              break;
          for ( i= 0; i < p[n]->n_n_flags; ++i )
          {
              p[n]->n_flags[i]= 0;
              p[n]->flags[i]= (flag_t*)malloc(sizeof(flag_t)*MAX_DATA_FLAGS);
              if ( p[n]->flags[i] == NULL ) eprintf("malloc failed in loadData");
          }
          j= 0;
      }

      //series number
      tmp= strtok(NULL, delims);

      //date
      tmp= strtok(NULL, delims);

      while ( tmp= strtok(NULL, delims) )
      {
          sscanf(tmp, "%d", &flag);
          if ( flag != 0 )
          {
              p[n]->flags[j][p[n]->n_flags[j]]= flag;
              p[n]->n_flags[j]= p[n]->n_flags[j] + 1;
          }
      }

      ++j;
      last_id= id;
    }

    // last station element flags
    {
        for ( i= 0; i < p[n]->n_n_flags; ++i )
        {
            p[n]->flags[i]= realloc(p[n]->flags[i], sizeof(flag_t)*(p[n]->n_flags[i]));
            if ( p[n]->flags[i] == NULL ) eprintf("realloc failed in loadData");
        }
    }

    tprintf("reading of flags finished\n");
    fclose(f);

    //READ SOURCES

    tprintf("reading of sources\n");
    strcpy(filename, SOURCES_DATA );
    f= fopen(filename, "rt");
    printf("%s\n", filename);

    last_id= -1;
    n= -1;
    while ( fgets(buffer, 1000, f) != NULL )
    {
      if ( buffer[0] == '%' )
        continue;

      tmp= strtok(buffer, delims);
      sscanf(tmp, "%d", &id);

      if ( last_id != id )
      {
          if ( last_id != -1 )
          {
              for ( i= 0; i < p[n]->n_n_sources; ++i )
              {
                  p[n]->sources[i]= realloc(p[n]->sources[i], sizeof(flag_t)*(p[n]->n_sources[i]));
                  if ( p[n]->sources[i] == NULL ) eprintf("realloc failed in loadData");
              }
          }

          ++n;
          if ( n == *n_stationElement2 )
              break;
          for ( i= 0; i < p[n]->n_n_sources; ++i )
          {
              p[n]->n_sources[i]= 0;
              p[n]->sources[i]= (flag_t*)malloc(sizeof(flag_t)*MAX_SOURCES);
              if ( p[n]->sources[i] == NULL ) eprintf("malloc failed in loadData");
          }
          j= 0;
      }

      //series number
      tmp= strtok(NULL, delims);

      //date
      tmp= strtok(NULL, delims);

      while ( tmp= strtok(NULL, delims) )
      {
          sscanf(tmp, "%d", &flag);
          if ( flag != 0 )
          {
              p[n]->sources[j][p[n]->n_sources[j]]= flag;
              ++(p[n]->n_sources[j]);
          }
      }

      ++j;
      last_id= id;
    }

    // last station element flags
    {
        for ( i= 0; i < p[n]->n_n_flags; ++i )
        {
            p[n]->flags[i]= realloc(p[n]->flags[i], sizeof(flag_t)*(p[n]->n_flags[i]));
            if ( p[n]->flags[i] == NULL ) eprintf("realloc failed in loadData");
        }
    }

    tprintf("reading station element data finished\n");
    fclose(f);
    tprintf("sorting data series\n");
    for ( i= 0; i < *n_stationElement2; ++i )
        sortSE2DataByDate((*se)[i]);
    tprintf("sorting of data series finished\n");
}

void loadData(stationSite2p** ss, int* n_stationSite2, stationElement2p** se, int* n_stationElement2)
{
    loadPreliminaryData();
    loadStationElement2(se, n_stationElement2);
    loadStationSite2(ss, n_stationSite2);
    shrinkSS2V(ss, n_stationSite2);
    matchStationElementsAndSites(ss, n_stationSite2, se, n_stationElement2);
}

void matchStationElementsAndSites(stationSite2p** ss, int* n_stationSite2, stationElement2p** se, int* n_stationElement2)
{
    tprintf("Matching %d station elements and %d station sites.\n", *n_stationElement2, *n_stationSite2);
    stationElement2p* sen= (stationElement2p*)malloc(sizeof(stationElement2p)*(*n_stationElement2));
    stationSite2p* ssn= (stationSite2p*)malloc(sizeof(stationSite2p)*(*n_stationSite2));
    int* mse= (int*)inalloc(*n_stationElement2);
    int* mss= (int*)inalloc(*n_stationSite2);
    seti(mse, *n_stationElement2, 0);
    seti(mss, *n_stationSite2, 0);

    int n= 0;
    int i, j;
    for ( i= 0; i < *n_stationSite2; ++i )
    {
        for ( j= 0; j < *n_stationElement2; ++j )
        {
            if ( (*ss)[i]->id == (*se)[j]->site )
            {
                if ( (mse)[j] )
                {
                    printf("%d %d\n", i,j);
                }
                else
                {
                    sen[n]= (*se)[j];
                    ssn[n]= (*ss)[i];
                    mse[j]= 1;
                    mss[i]= 1;
                    ++n;
                    break;
                }
            }
        }
    }
    for ( i= 0; i < *n_stationElement2; ++i )
        if ( !mse[i] )
            destroySE2((*se)[i]);
    for ( i= 0; i < *n_stationSite2; ++i )
        if ( !mss[i] )
            destroySS2((*ss)[i]);
    free(*ss);
    free(*se);
    *se= realloc(sen, n*sizeof(stationElement2p));
    *ss= realloc(ssn, n*sizeof(stationSite2p));
    *n_stationSite2= n;
    *n_stationElement2= n;
    free(mse);
    free(mss);
    tprintf("Size after matching: %d.\n", n);
}
