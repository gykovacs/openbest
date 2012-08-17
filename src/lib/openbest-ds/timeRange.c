#include "openbest-ds/timeRange.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

timeRange createTimeRangeN()
{
  timeRange tr;
  tr.first= createTimeInstantN(0, 0, 0, 0, 0, 0);
  tr.last= createTimeInstantN(0, 0, 0, 0, 0, 0);
  return tr;
}

timeRange createTimeRangeN1a(timeInstant* t1)
{
  timeRange tr;
  tr.first= *t1;
  tr.last= addIntervalN(t1, 1, TI_YEAR);
  return tr;
}

timeRange createTimeRangeN1b(real t1)
{
  timeRange tr;
  convertFromSerialTime(t1, &(tr.first));
  tr.last= addIntervalN(&(tr.first), 1, TI_YEAR);
  return tr;
}

timeRange createTimeRangeN2a(timeInstant* t1, timeInstant* t2)
{
  timeRange tr;
  tr.first= *t1;
  tr.last= *t2;
  return tr;
}

timeRange createTimeRangeN2b(real t1, real t2)
{
  timeRange tr;
  convertFromSerialTime(t1, &(tr.first));
  convertFromSerialTime(t2, &(tr.last));
}

timeRange createTimeRangeN3(real y, real m, real d)
{
  timeRange tr;
  setTI(&(tr.first), y, m, d, 0, 0, 0);
  tr.last= addIntervalN(&(tr.first), 1, TI_DAY);
  return tr;
}

timeRange createTimeRangeN4(real y, real m, real d, real h)
{
  timeRange tr;
  setTI(&(tr.first), y, m, d, h, 0, 0);
  tr.last= addIntervalN(&(tr.first), 1, TI_HOUR);
  return tr;
}

timeRange createTimeRangeN5(real y, real m, real d, real h, real min)
{
  timeRange tr;
  setTI(&(tr.first), y, m, d, h, min, 0);
  tr.last= addIntervalN(&(tr.first), 1, TI_MIN);
  return tr;
}

timeRange createTimeRangeN6(real y, real m, real d, real h, real min, real s)
{
  timeRange tr;
  setTI(&(tr.first), y, m, d, h, min, s);
  tr.last= addIntervalN(&(tr.first), 1, TI_SEC);
  return tr;
}

timeRange* createTimeRangeP()
{
  timeRange* tr= (timeRange*)malloc(sizeof(timeRange));
  tr->first= createTimeInstantN(0, 0, 0, 0, 0, 0);
  tr->last= createTimeInstantN(0, 0, 0, 0, 0, 0);
  return tr;
}

timeRange* createTimeRangeP1a(timeInstant* t1)
{
  timeRange* tr= createTimeRangeP();
  tr->first= *t1;
  tr->last= addIntervalN(&(tr->first), 1, TI_YEAR);
  return tr;
}

timeRange* createTimeRangeP1b(real t1)
{
  timeRange* tr= createTimeRangeP();
  convertFromSerialTime(t1, &(tr->first));
  tr->last= addIntervalN(&(tr->first), 1, TI_YEAR);
  return tr;
}

timeRange* createTimeRangeP2a(timeInstant* t1, timeInstant* t2)
{
  timeRange* tr= createTimeRangeP();
  tr->first= *t1;
  tr->last= *t2;
  return tr;
}

timeRange* createTimeRangeP2b(real t1, real t2)
{
  timeRange* tr= createTimeRangeP();
  convertFromSerialTime(t1, &(tr->first));
  convertFromSerialTime(t2, &(tr->last));
}

timeRange* createTimeRangeP3(real y, real m, real d)
{
  timeRange* tr= createTimeRangeP();
  setTI(&(tr->first), y, m, d, 0, 0, 0);
  tr->last= addIntervalN(&(tr->first), 1, TI_DAY);
  return tr;
}

timeRange* createTimeRangeP4(real y, real m, real d, real h)
{
  timeRange* tr= createTimeRangeP();
  setTI(&(tr->first), y, m, d, h, 0, 0);
  tr->last= addIntervalN(&(tr->first), 1, TI_HOUR);
  return tr;
}

timeRange* createTimeRangeP5(real y, real m, real d, real h, real min)
{
  timeRange* tr= createTimeRangeP();
  setTI(&(tr->first), y, m, d, h, min, 0);
  tr->last= addIntervalN(&(tr->first), 1, TI_MIN);
  return tr;
}

timeRange* createTimeRangeP6(real y, real m, real d, real h, real min, real s)
{
  timeRange* tr= createTimeRangeP();
  setTI(&(tr->first), y, m, d, h, min, s);
  tr->last= addIntervalN(&(tr->first), 1, TI_SEC);
  return tr;
}

void displayTR(timeRange* tr)
{
  displayTI(&(tr->first));
  printf(" - ");
  displayTI(&(tr->last));
}

void destroyTR(timeRange* tr)
{
  free(tr);
}
