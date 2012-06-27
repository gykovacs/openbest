#ifndef _STATION_SITE_H_
#define _STATION_SITE_H_

struct stationSite
{
    int id;
    int* other_ids;
    char[100] country;
    char[100] state;
    char[100] county;
    char[100] location;
    char[100]* all_locations;
    char[100]* all_location_times;
    char[10] timezone;
    char[20] primary_name;
    //char
    
};

#endif