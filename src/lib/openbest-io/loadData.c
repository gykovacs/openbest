#include <string.h>

#include "openbest-ds/config-ds.h"
#include "openbest-io/loadData.h"

#define MAX_LENGTH_OF_TIME_SERIES 4000
#define MAX_DATA_FLAGS 30

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
    strcpy(filename, DATA_DIR );
    strcat(filename, SITE_DATA );
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
      p[n]->country= lookupValuePKT(&countryNamesPKT, tmpstr);
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

    strcpy(filename, DATA_DIR );
    strcat(filename, SITE_FLAGS );

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
    printf("reading flags finished\n"); fflush(stdout);
    fclose(f);
}


void loadStationElement2(stationElement2p** se, int* n_stationElement2)
{
    printf("reading station elements\n"); fflush(stdout);

    char filename[100];
    strcpy(filename, DATA_DIR );
    strcat(filename, TEMP_DATA );
    FILE* f= fopen(filename, "rt");

    char buffer[1000];
    char tmpstr[100];

    printf("%s\n", filename);
    char* tmp;
    char delims[]= "\t";

    stationElement2p* p= (stationElement2p*)malloc(sizeof(stationElement2p)*10000);
    int capacity= 10000, n= -1;

    int id;
    real* datesp;
    temp_t* tempp;
    real* uncp;
    short* obsp;
    char* tobp;
    int tsn;
    int last_id= -1;
    float rtmp;
    float t1, t2, t3;
    int t0, t4, t5;

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
      //sscanf(buffer, "%d%d%f%f%f%d%d", &id, &t0, &t1, &t2, &t3, &t4, &t5);
      if ( last_id != id )
      {
          if ( last_id != -1 )
          {
              datesp= (real*)realloc(datesp, sizeof(real)*tsn);
              if ( datesp == NULL ) eprintf("realloc failed in loadData");
              tobp= (char*)realloc(tobp, sizeof(char)*tsn);
              if ( tobp == NULL ) eprintf("realloc failed in loadData");
              tempp= (temp_t*)realloc(tempp, sizeof(temp_t)*tsn);
              if ( tempp == NULL ) eprintf("realloc failed in loadData");
              uncp= (real*)realloc(uncp, sizeof(real)*tsn);
              if ( uncp == NULL ) eprintf("realloc failed in loadData");
              obsp= (short*)realloc(obsp, sizeof(short)*tsn);
              if ( obsp == NULL ) eprintf("realloc failed in loadData");
              p[n]->dates= datesp;
              p[n]->time_of_observation= tobp;
              p[n]->data= tempp;
              p[n]->num_measurements= obsp;
              p[n]->uncertainty= uncp;
              p[n]->n_flags= (char*)malloc(sizeof(char)*tsn);
              if ( p[n]->n_flags == NULL ) eprintf("malloc failed in loadData");
              p[n]->flags= (flag_t**)malloc(sizeof(flag_t*)*tsn);
              if ( p[n]->flags == NULL ) eprintf("malloc failed in loadData");
              p[n]->n_dates= p[n]->n_time_of_observation= p[n]->n_data= p[n]->n_num_measurements= p[n]->n_uncertainty= p[n]->n_n_flags= tsn;
              //printf(":%d ", tsn); fflush(stdout);
          }

          ++n;
          p[n]= (stationSite2p)createSE2N();
          datesp= (real*)malloc(sizeof(real)*MAX_LENGTH_OF_TIME_SERIES);
          if ( datesp == NULL ) eprintf("malloc failed in loadData");
          tempp= (temp_t*)malloc(sizeof(temp_t)*MAX_LENGTH_OF_TIME_SERIES);
          if ( tempp == NULL ) eprintf("malloc failed in loadData");
          uncp= (real*)malloc(sizeof(real)*MAX_LENGTH_OF_TIME_SERIES);
          if ( uncp == NULL ) eprintf("malloc failed in loadData");
          obsp= (short*)malloc(sizeof(short*)*MAX_LENGTH_OF_TIME_SERIES);
          if ( obsp == NULL ) eprintf("malloc failed in loadData");
          tobp= (char*)malloc(sizeof(char*)*MAX_LENGTH_OF_TIME_SERIES);
          if ( tobp == NULL ) eprintf("malloc failed in loadData");
          tsn= 0;
          p[n]->site= id;
          //printf("%d", n); fflush(stdout);
      }

      //series number
      tmp= strtok(NULL, delims);

      //date
      tmp= strtok(NULL, delims);
      sscanf(tmp, "%f", &rtmp);
      datesp[tsn]= rtmp;
      //datesp[tsn]= t1;

      //temperature
      tmp= strtok(NULL, delims);
      sscanf(tmp, "%f", &rtmp);
      tempp[tsn]= rtmp;
      //tempp[tsn]= t2;

      //uncertainty
      tmp= strtok(NULL, delims);
      sscanf(tmp, "%f", &rtmp);
      uncp[tsn]= rtmp;
      //uncp[tsn]= t3;

      //observations
      tmp= strtok(NULL, delims);
      sscanf(tmp, "%d", &(obsp[tsn]));
      //obsp[tsn]= t4;

      //time of observation
      tmp= strtok(NULL, delims);
      sscanf(tmp, "%d", &(tobp[tsn]));
      //tobp[tsn]= t5;

      ++tsn;
      //printf("%d ", n); fflush(stdout);
      last_id= id;
    }

    // last station element
    {
        datesp= (real*)realloc(datesp, sizeof(real)*tsn);
        if ( datesp == NULL ) eprintf("realloc failed in loadData");
        tobp= (char*)realloc(tobp, sizeof(char)*tsn);
        if ( tobp == NULL ) eprintf("realloc failed in loadData");
        tempp= (temp_t*)realloc(tempp, sizeof(temp_t)*tsn);
        if ( tempp == NULL ) eprintf("realloc failed in loadData");
        uncp= (real*)realloc(uncp, sizeof(real)*tsn);
        if ( uncp == NULL ) eprintf("realloc failed in loadData");
        obsp= (short*)realloc(obsp, sizeof(short)*tsn);
        if ( obsp == NULL ) eprintf("realloc failed in loadData");
        p[n]->dates= datesp;
        p[n]->time_of_observation= tobp;
        p[n]->data= tempp;
        p[n]->num_measurements= obsp;
        p[n]->uncertainty= uncp;
        p[n]->n_flags= (char*)malloc(sizeof(char)*tsn);
        if ( p[n]->n_flags == NULL ) eprintf("malloc failed in loadData");
        p[n]->flags= (flag_t**)malloc(sizeof(flag_t*)*tsn);
        if ( p[n]->flags == NULL ) eprintf("malloc failed in loadData");
        p[n]->n_dates= p[n]->n_time_of_observation= p[n]->n_data= p[n]->n_num_measurements= p[n]->n_uncertainty= p[n]->n_n_flags= tsn;
       // printf(":%d ", tsn); fflush(stdout);
    }

    *se= p;
    *n_stationElement2= n + 1;

    fclose(f);

    printf("read station element data finished\n"); fflush(stdout);

    // READING FLAGS

    strcpy(filename, DATA_DIR );
    strcat(filename, TEMP_FLAGS );
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

      //printf("%d ", id); fflush(stdout);
      if ( last_id != id )
      {
          if ( last_id != -1 )
          {
              //printf("%d,%d ", n, p[n]->n_n_flags); fflush(stdout);
              for ( i= 0; i < p[n]->n_n_flags; ++i )
              {
                  p[n]->flags[i]= realloc(p[n]->flags[i], sizeof(flag_t)*(p[n]->n_flags[i]));
                  if ( p[n]->flags[i] == NULL ) eprintf("realloc failed in loadData");
              }
          }
          //printf("c%d", p[id]->n_n_flags); fflush(stdout);
          //printf(" %d %d\n", sizeof(*p[id]->n_flags), sizeof(p[id+1]->n_flags));
          ++n;
          if ( n == n_stationElement2 )
              break;
          for ( i= 0; i < p[n]->n_n_flags; ++i )
          {
              p[n]->n_flags[i]= 0;
              //printf("."); fflush(stdout);
              p[n]->flags[i]= (flag_t*)malloc(sizeof(flag_t)*MAX_DATA_FLAGS);
              if ( p[n]->flags[i] == NULL ) eprintf("malloc failed in loadData");
          }
          j= 0;
      }
      //printf("b"); fflush(stdout);

      //series number
      tmp= strtok(NULL, delims);

      //date
      tmp= strtok(NULL, delims);

      //printf("a"); fflush(stdout);

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
        //printf("%d,%d ", n, p[n]->n_n_flags); fflush(stdout);
        for ( i= 0; i < p[n]->n_n_flags; ++i )
        {
            p[n]->flags[i]= realloc(p[n]->flags[i], sizeof(flag_t)*(p[n]->n_flags[i]));
            if ( p[n]->flags[i] == NULL ) eprintf("realloc failed in loadData");
        }
    }

    printf("finished\n"); fflush(stdout);
    fclose(f);
}

void loadData(stationSite2p** ss, int* n_stationSite2, stationElement2p** se, int* n_stationElement2)
{
    loadPreliminaryData();
    loadStationElement2(se, n_stationElement2);
    loadStationSite2(ss, n_stationSite2);
    shrinkSS2V(ss, n_stationSite2);
}
