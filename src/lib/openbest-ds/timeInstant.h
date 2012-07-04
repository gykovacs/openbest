#ifndef _TIME_INSTANT_H_
#define _TIME_INSTANT_H_

#include <float.h>

#include "openbest-ds/config.h"

typedef struct
{
  real year;
  real month;
  real day;
  real hour;
  real minute;
  real second;
  real yearnum;

} timeInstant;

timeInstant createTimeInstantN(real year, real month, real day, real hour, real minute, real second);

timeInstant* createTimeInstantP(real year, real month, real day, real hour, real minute, real second);

timeInstant* createTimeInstantC(timeInstant* ti);

bool checkTI(timeInstant* t);

void displayTI(timeInstant* t);

int daysInMonth(timeInstant* t);

real datenum(timeInstant* t);

#endif