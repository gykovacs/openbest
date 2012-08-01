#ifndef _TIME_INSTANT_H_
#define _TIME_INSTANT_H_

#include <float.h>

#include "openbest-ds/config-ds.h"

/**
 * structure to represent a moment
 */ 
typedef struct
{
  /** year field*/
  real year;
  /** month field*/
  real month;
  /** day field*/
  real day;
  /** hour field*/
  real hour;
  /** minute field*/
  real minute;
  /** second field*/
  real second;
  real yearnum;

} timeInstant;

/**
 * global constants
 */ 
enum
{
  TI_DAY,
  TI_HOUR,
  TI_MIN,
  TI_SEC,
  TI_MON,
  TI_YEAR
};

/**
 * returns a new timeInstant object with the given parameters
 * @param year year of date
 * @param month month of date
 * @param day day of date
 * @param hour hour of moment
 * @param minute minute of moment
 * @param second second of moment
 * @returns new timeInstant object by value
 */ 
timeInstant createTimeInstantN(real year, real month, real day, real hour, real minute, real second);

/**
 * returns a new timeInstant object with the given parameters
 * @param year year of date
 * @param month month of date
 * @param day day of date
 * @param hour hour of moment
 * @param minute minute of moment
 * @param second second of moment
 * @returns new timeInstant object by pointer
 */
timeInstant* createTimeInstantP(real year, real month, real day, real hour, real minute, real second);

/**
 * copys the parameter and returns a new instance
 * @param ti instance to copy
 * @returns new timeInstant object by pointer
 */
timeInstant* createTimeInstantC(timeInstant* ti);

/**
 * creates timeInstant object from serial time
 * @param t serial time value
 * @returns new timeInstant object by pointer
 */
timeInstant* createFromSerialTime(real t);

/**
 * sets the field of the parameter timeInstant to the rest of the parameters
 * @param ti object to set the fields of
 * @param year year of date
 * @param month month of date
 * @param day day of date
 * @param hour hour of moment
 * @param minute minute of moment
 * @param second second of moment
 */
void setTI(timeInstant* ti, real year, real month, real day, real hour, real minute, real second);

/**
 * converts time from serial to timeInstant
 * @param t serial time value
 * @param ti timeInstant objects, that's fields are set
 */ 
void convertFromSerialTime(real t, timeInstant* ti);

/**
 * checks the validity of the timeInstant values
 * @param t timeInstant to check the validity of
 * @returns boolean value, true - if valid; false - otherwise
 */ 
bool checkTI(timeInstant* t);

/**
 * displays the timeInstant on the standard output
 * @param t timeInstant to display
 */ 
void displayTI(timeInstant* t);

/**
 * returns the number of days in the month specified by the timeInstant parameter
 * @param t timeInstant parameter
 * @returns the number of days in the month
 */ 
int daysInMonth(timeInstant* t);

/**
 * returns the number of days until the month parameter in the given year
 * @param month month parameter
 * @param year year parameter
 * @returns the number of days until month.01.year
 */
int daysUntilMonth(int month, int year);

/**
 * converts the timeInstant parameter to serial time; reference date: 0000.01.01-00:00:00.
 * @param t timeInstant parameter to convert
 * @returns serial time
 */ 
real datenum(timeInstant* t);

/**
 * comparison operator
 * @param t1 timeInstant parameter
 * @param t2 timeInstant parameter
 * @returns t1 == t2
 */ 
bool eqTI(timeInstant* t1, timeInstant* t2);

/**
 * comparison operator
 * @param t1 timeInstant parameter
 * @param t2 timeInstant parameter
 * @returns t1 >= t2
 */
bool geTI(timeInstant* t1, timeInstant* t2);

/**
 * comparison operator
 * @param t1 timeInstant parameter
 * @param t2 timeInstant parameter
 * @returns t1 > t2
 */
bool gtTI(timeInstant* t1, timeInstant* t2);

/**
 * comparison operator
 * @param t1 timeInstant parameter
 * @param t2 timeInstant parameter
 * @returns t1 <= t2
 */
bool leTI(timeInstant* t1, timeInstant* t2);

/**
 * comparison operator
 * @param t1 timeInstant parameter
 * @param t2 timeInstant parameter
 * @returns t1 < t2
 */
bool ltTI(timeInstant* t1, timeInstant* t2);

/**
 * adds interval to the timeInstant parameter
 * @param t timeInstant parameter to add to
 * @param value length of interval
 * @param type type of the interval
 */ 
void addInterval(timeInstant* t, real value, int type);

/**
 * adds interval to the timeInstant parameter and returns a new instance by value
 * @param t timeInstant parameter to add to
 * @param value length of interval
 * @param type type of the interval
 * @returns new timeInstant object by value
 */ 
timeInstant addIntervalN(timeInstant* t, real value, int type);

/**
 * adds interval to the timeInstant parameter and returns a new instance by pointer
 * @param t timeInstant parameter to add to
 * @param value length of interval
 * @param type type of the interval
 * @returns new timeInstant object by pointer
 */ 
timeInstant* addIntervalP(timeInstant* t, real value, int type);

real monthNum(real year);

real yearNum(real monthNum);

#endif
