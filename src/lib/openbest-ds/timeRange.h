#ifndef _TIME_RANGE_H_
#define _TIME_RANGE_H_

#include "openbest-ds/timeInstant.h"

/**
 * structure to represent time ranges
 */ 
typedef struct
{
  /** beginning of the range */
  timeInstant first;
  /** end of the range */
  timeInstant last;
} timeRange;

/**
 * creates a new timeRange object and returns it by value
 * @returns new timeRange object by value
 */ 
timeRange createTimeRangeN();

/**
 * creates a new timeRange object and initializes the first moment by parameter t1, the last moment is t1 + 1 year
 * @param t1 first moment
 * @returns new timeRange object by value
 */
timeRange createTimeRangeN1a(timeInstant* t1);

/**
 * creates a new timeRange object and initializes the first moment by parameter t1, the last moment is t1 + 1 year
 * @param t1 first moment
 * @returns new timeRange object by value
 */
timeRange createTimeRangeN1b(real t1);

/**
 * creates a new timeRange object and initializes the first and last moments by parameters t1 and t2
 * @param t1 first moment
 * @param t2 last moment
 * @returns new timeRange object by value
 */
timeRange createTimeRangeN2a(timeInstant* t1, timeInstant* t2);

/**
 * creates a new timeRange object and initializes the first and last moments by parameters t1 and t2
 * @param t1 first moment
 * @param t2 last moment
 * @returns new timeRange object by value
 */
timeRange createTimeRangeN2b(real t1, real t2);

/**
 * creates a new timeRange object and initializes the first moment by parameters m.d.y-0:0:0, last moment is m.d+1.y-0:0:0
 * @param y year of first date
 * @param m month of first date
 * @param d day of first date
 * @returns new timeRange object by value
 */
timeRange createTimeRangeN3(real y, real m, real d);

/**
 * creates a new timeRange object and initializes the first moment by parameters m.d.y-h:0:0, last moment is m.d.y-h+1:0:0
 * @param y year of first date
 * @param m month of first date
 * @param d day of first date
 * @param h hour of first moment
 * @returns new timeRange object by value
 */
timeRange createTimeRangeN4(real y, real m, real d, real h);

/**
 * creates a new timeRange object and initializes the first moment by parameters m.d.y-h:min:0, last moment is m.d.y-h:min+1:0
 * @param y year of first date
 * @param m month of first date
 * @param d day of first date
 * @param h hour of first moment
 * @param min minute of first moment
 * @returns new timeRange object by value
 */
timeRange createTimeRangeN5(real y, real m, real d, real h, real min);

/**
 * creates a new timeRange object and initializes the first moment by parameters m.d.y-h:min:s, last moment is m.d.y-h:min:s+1
 * @param y year of first date
 * @param m month of first date
 * @param d day of first date
 * @param h hour of first moment
 * @param min minute of first moment
 * @param s second of first moment
 * @returns new timeRange object by value
 */
timeRange createTimeRangeN6(real y, real m, real d, real h, real min, real s);

/**
 * creates a new timeRange object and returns it by pointer
 * @returns new timeRange object by pointer
 */ 
timeRange* createTimeRangeP();

/**
 * creates a new timeRange object and initializes the first moment by parameter t1, the last moment is t1 + 1 year
 * @param t1 first moment
 * @returns new timeRange object by pointer
 */
timeRange* createTimeRangeP1a(timeInstant* t1);

/**
 * creates a new timeRange object and initializes the first moment by parameter t1, the last moment is t1 + 1 year
 * @param t1 first moment
 * @returns new timeRange object by pointer
 */
timeRange* createTimeRangeP1b(real t1);

/**
 * creates a new timeRange object and initializes the first and last moments by parameters t1 and t2
 * @param t1 first moment
 * @param t2 last moment
 * @returns new timeRange object by pointer
 */
timeRange* createTimeRangeP2a(timeInstant* t1, timeInstant* t2);

/**
 * creates a new timeRange object and initializes the first and last moments by parameters t1 and t2
 * @param t1 first moment
 * @param t2 last moment
 * @returns new timeRange object by pointer
 */
timeRange* createTimeRangeP2b(real t1, real t2);

/**
 * creates a new timeRange object and initializes the first moment by parameters m.d.y-0:0:0, last moment is m.d+1.y-0:0:0
 * @param y year of first date
 * @param m month of first date
 * @param d day of first date
 * @returns new timeRange object by pointer
 */
timeRange* createTimeRangeP3(real y, real m, real d);

/**
 * creates a new timeRange object and initializes the first moment by parameters m.d.y-h:0:0, last moment is m.d.y-h+1:0:0
 * @param y year of first date
 * @param m month of first date
 * @param d day of first date
 * @param h hour of first moment
 * @returns new timeRange object by pointer
 */
timeRange* createTimeRangeP4(real y, real m, real d, real h);

/**
 * creates a new timeRange object and initializes the first moment by parameters m.d.y-h:min:0, last moment is m.d.y-h:min+1:0
 * @param y year of first date
 * @param m month of first date
 * @param d day of first date
 * @param h hour of first moment
 * @param min minute of first moment
 * @returns new timeRange object by pointer
 */
timeRange* createTimeRangeP5(real y, real m, real d, real h, real min);

/**
 * creates a new timeRange object and initializes the first moment by parameters m.d.y-h:min:s, last moment is m.d.y-h:min:s+1
 * @param y year of first date
 * @param m month of first date
 * @param d day of first date
 * @param h hour of first moment
 * @param min minute of first moment
 * @param s second of first moment
 * @returns new timeRange object by pointer
 */
timeRange* createTimeRangeP6(real y, real m, real d, real h, real min, real s);

/**
 * displays the parameter timeRange on stdout
 * @param tr timeRange object to display
 */ 
void displayTR(timeRange* tr);

/**
 * destroys the parameter timeRange object
 * @param tr timeRange object to destroy
 */ 
void destroyTR(timeRange* tr);

#endif