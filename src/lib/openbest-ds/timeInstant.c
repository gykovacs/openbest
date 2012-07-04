#include "openbest-ds/timeInstant.h"

timeInstant createTimeInstantN(real year, real month, real day, real hour, real minute, real second)
{
  timeInstant ti;
  ti.year= year;
  ti.month= month;
  ti.day= day;
  ti.hour= hour;
  ti.minute= minute;
  ti.second= second;
  
  return ti;
}

timeInstant* createTimeInstantP(real year, real month, real day, real hour, real minute, real second)
{
  timeInstant* ti= (timeInstant*)malloc(sizeof(timeInstant));
  ti->year= year;
  ti->month= month;
  ti->day= day;
  ti->hour= hour;
  ti->minute= minute;
  ti->second= second;
}

timeInstant* createTimeInstantC(timeInstant* ti)
{
  return createTimeInstantP(ti->year, ti->month, ti->day, ti->hour, ti->minute, ti->second);
}

bool checkTI(timeInstant* ti)
{
  if ( daysInMonth(ti) < ti.day || ti.day < 1 )
  {
    eprintf("Specified day is out of bounds.");
    return false;
  }
  if ( ti.hour >= 24 || ti.hour < 0 )
  {
    eprintf("Specified hour is out of bounds.");
    return false;
  }
  if ( ti.month > 12 || ti.month < 1 )
  {
    eprintf("Specified month is out of bounds.");
    return false;
  }
  if ( ti.minute >= 60 || ti.minute < 0 )
  {
    eprintf("Specified minute is out of bounds.");
    return false;
  }
  if ( ti.second >= 60 || ti.second < 0 )
  {
    eprintf("Specified second is out of bounds.");
    return false;
  }
  return true;
}

void displayTI(timeInstant* ti)
{
  printf("%4f:%2f:%2f:%2f:%2f:%2f\n", ti->year, ti->month, ti->day, ti->hour, ti->minute, ti->second);
}

int daysInMonth(timeInstant* ti)
{
  if ( ti->months == 2 && (int)(ti->years) % 4 == 0 && ((int)(ti->years) % 100 != 0 || (int)(ti->years) % 400 == 0 )) )
    return 29;
  switch(ti->month)
  {
    case 1:
      return 31;
    case 2:
      return 28;
    case 3:
      return 31;
    case 4:
      return 30;
    case 5:
      return 31;
    case 6:
      return 30;
    case 7:
      return 31;
    case 8:
      return 31;
    case 9:
      return 30;
    case 10:
      return 31;
    case 11:
      return 30;
    case 12:
      return 31;
  }
}

real datenum(timeInstant* t)
{
  
}