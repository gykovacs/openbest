#include "openbest-ds/timeInstant.h"
#include <stdlib.h>
#include <math.h>

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
  if ( daysInMonth(ti) < ti->day || ti->day < 1 )
  {
    eprintf("Specified day is out of bounds.");
    return false;
  }
  if ( ti->hour >= 24 || ti->hour < 0 )
  {
    eprintf("Specified hour is out of bounds.");
    return false;
  }
  if ( ti->month > 12 || ti->month < 1 )
  {
    eprintf("Specified month is out of bounds.");
    return false;
  }
  if ( ti->minute >= 60 || ti->minute < 0 )
  {
    eprintf("Specified minute is out of bounds.");
    return false;
  }
  if ( ti->second >= 60 || ti->second < 0 )
  {
    eprintf("Specified second is out of bounds.");
    return false;
  }
  return true;
}

void displayTI(timeInstant* ti)
{
  printf("%04.0f-%02.0f-%02.0f %02.0f:%02.0f:%02.0f", ti->year, ti->month, ti->day, ti->hour, ti->minute, ti->second);
}

int daysInMonth(timeInstant* ti)
{
  if ( ti->month == 2 && (int)(ti->year) % 4 == 0 && ((int)(ti->year) % 100 != 0 || (int)(ti->year) % 400 == 0 ) )
    return 29;
  switch((int)(ti->month))
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

int daysUntilMonth(int month, int year)
{
  int sum= 0;
  switch(month)
  {
    case 12: sum+= 30;
    case 11: sum+= 31;
    case 10: sum+= 30;
    case 9: sum+= 31;
    case 8: sum+= 31;
    case 7: sum+= 30;
    case 6: sum+= 31;
    case 5: sum+= 30;
    case 4: sum+= 31;
    case 3: 
      if ( year % 4 == 0  && (year % 100 != 0 || year % 400 == 0 ) )
	sum+= 29;
      else
	sum+= 28;
    case 2: sum+= 31;
    default:
      return sum;
  }
}

real datenum(timeInstant* t)
{
  real day= 365 * t->year;
  int extra= (int)((t->year-1) / 4);
  int extraextra= (int)((t->year-1) / 100);
  int extraextraextra= (int)((t->year-1) / 400);
  day+= extra - extraextra + extraextraextra + daysUntilMonth(t->month, t->year) + t->day;
  if ( t->year > 0 )
    day+= 1;
  
  real fraction= ((t->hour) * 3600 + (t->minute) * 60 + t->second)/(24*3600);
  
  return day + fraction;
}

bool eqTI(timeInstant* t1, timeInstant* t2)
{
  return datenum(t1) == datenum(t2);
}

bool geTI(timeInstant* t1, timeInstant* t2)
{
  return datenum(t1) >= datenum(t2);
}

bool gtTI(timeInstant* t1, timeInstant* t2)
{
  return datenum(t1) > datenum(t2);
}
bool leTI(timeInstant* t1, timeInstant* t2)
{
  return datenum(t1) <= datenum(t2);
}

bool ltTI(timeInstant* t1, timeInstant* t2)
{
  return datenum(t1) < datenum(t2);
}

void addInterval(timeInstant* t, real value, int type)
{
  real v0= datenum(t);
  switch(type)
  {
    case TI_DAY:
      v0+= value;
      convertFromSerialTime(v0, t);
      break;
    case TI_HOUR:
      v0+= value/24.0;
      convertFromSerialTime(v0, t);
      break;
    case TI_MIN:
      v0+= value/(24.0*60);
      convertFromSerialTime(v0, t);
      break;
    case TI_SEC:
      v0+= value/(24.0*3600);
      convertFromSerialTime(v0, t);
      break;
    case TI_MON:
      t->month+= value;
      while ( t->month > 12 )
      {
	t->month-= 12;
	t->year++;
      }
      while ( t->month < 1 )
      {
	t->month+= 12;
	t->year--;
      }
      break;
    case TI_YEAR:
      t->year+= value;
      break;
  }
}

timeInstant addIntervalN(timeInstant* t, real value, int type)
{
  timeInstant ti;
  ti= *t;
  
  real v0= datenum(t);
  switch(type)
  {
    case TI_DAY:
      v0+= value;
      convertFromSerialTime(v0, &ti);
      break;
    case TI_HOUR:
      v0+= value/24.0;
      convertFromSerialTime(v0, &ti);
      break;
    case TI_MIN:
      v0+= value/(24.0*60);
      convertFromSerialTime(v0, &ti);
      break;
    case TI_SEC:
      v0+= value/(24.0*3600);
      convertFromSerialTime(v0, &ti);
      break;
    case TI_MON:
      ti.month+= value;
      while ( ti.month > 12 )
      {
	ti.month-= 12;
	ti.year++;
      }
      while ( ti.month < 1 )
      {
	ti.month+= 12;
	ti.year--;
      }
      break;
    case TI_YEAR:
      ti.year+= value;
      break;
  }
  
  return ti;
}

timeInstant* addIntervalP(timeInstant* t, real value, int type)
{
  timeInstant* ti= createTimeInstantC(t);
  
  real v0= datenum(t);
  switch(type)
  {
    case TI_DAY:
      v0+= value;
      convertFromSerialTime(v0, ti);
      break;
    case TI_HOUR:
      v0+= value/24.0;
      convertFromSerialTime(v0, ti);
      break;
    case TI_MIN:
      v0+= value/(24.0*60);
      convertFromSerialTime(v0, ti);
      break;
    case TI_SEC:
      v0+= value/(24.0*3600);
      convertFromSerialTime(v0, ti);
      break;
    case TI_MON:
      ti->month+= value;
      while ( ti->month > 12 )
      {
	ti->month-= 12;
	ti->year++;
      }
      while ( ti->month < 1 )
      {
	ti->month+= 12;
	ti->year--;
      }
      break;
    case TI_YEAR:
      ti->year+= value;
      break;
  }
  
  return ti;
}

void convertFromSerialTime(real t, timeInstant* ti)
{
  int day= t;
  int year= day/365.25;
  int rest= day - year*365;
  int extra= year / 4;
  int extraextra= year / 100;
  int extraextraextra= year / 400;
  
  rest= rest - extra + extraextra - extraextraextra;
  if ( year > 0 )
    day-= 1;
  
  int month= 1;
  int d1= daysUntilMonth(1,year);
  int d2= daysUntilMonth(2,year);
  int d3= daysUntilMonth(3,year);
  int d4= daysUntilMonth(4,year);
  int d5= daysUntilMonth(5,year);
  int d6= daysUntilMonth(6,year);
  int d7= daysUntilMonth(7,year);
  int d8= daysUntilMonth(8,year);
  int d9= daysUntilMonth(9,year);
  int d10= daysUntilMonth(10,year);
  int d11= daysUntilMonth(11,year);
  int d12= daysUntilMonth(12,year);
  
  if ( rest < d2 )
  {
    rest-= d1;
    month= 1;
  }
  else if ( rest < d3 )
  {
    rest-= d2;
    month= 2;
  }
  else if ( rest < d4 )
  {
    rest-= d3;
    month= 3;
  }
  else if ( rest < d5 )
  {
    rest-= d4;
    month= 4;
  }
  else if ( rest < d6 )
  {
    rest-= d5;
    month= 5;
  }
  else if ( rest < d7 )
  {
    rest-= d6;
    month= 6;
  }
  else if ( rest < d8 )
  {
    rest-= d7;
    month= 7;
  }
  else if ( rest < d9 )
  {
    rest-= d8;
    month= 8;
  }
  else if ( rest < d10 )
  {
    rest-= d9;
    month= 9;
  }
  else if ( rest < d11 )
  {
    rest-= d10;
    month= 10;
  }
  else if ( rest < d12 )
  {
    rest-= d11;
    month= 11;
  }
  ti->year= year;
  ti->month= month;
  ti->day= rest;
  
  real fraction= (t-floor(t)) * 24*3600;
  int hour= fraction/3600;
  int minute= (fraction - hour*3600)/60;
  int second= fraction - hour*3600 - minute*60;
  ti->hour= hour;
  ti->minute= minute;
  ti->second= second;
}

timeInstant* createFromSerialTime(real t)
{
  timeInstant* ti= createTimeInstantP(0, 0, 0, 0, 0, 0);
  convertFromSerialTime(t, ti);
  return ti;
}

void setTI(timeInstant* ti, real year, real month, real day, real hour, real minute, real second)
{
  ti->year= year;
  ti->month= month;
  ti->day= day;
  ti->hour= hour;
  ti->minute= minute;
  ti->second= second;
}

real monthNum(real year)
{
    return floor((year - 1600 + 1.0/24.0) * 12.0 + 0.5);
}

real yearNum(real month)
{
    return (month / 12.0) - 1.0/24.0 + 1600;
}
