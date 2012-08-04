#include "openbest-ds/stationManifest.h"

void setStartDateSM(stationManifest* sm, timeInstant* ti)
{
    sm->duration= createTimeRangeN1a(ti);
}

void setDurationSM2(stationManifest* sm, timeInstant* t1, timeInstant* t2)
{

}

void setDurationSM1(stationManifest* sm, timeRange* tr)
{

}
