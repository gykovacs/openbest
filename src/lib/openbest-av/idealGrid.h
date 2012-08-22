#ifndef _IDEAL_GRID_H_
#define _IDEAL_GRID_H_

/**
  * idealGrid function from the Matlab-BEST code
  * @param num number of grid points
  * @param latitude pointer to a real* variable for latitude output
  * @param n_latitude pointer to an int variable - length of latitude
  * @param longitude pointer to a real* variable for longitude output
  * @param n_longitude pointer to an int variable - length of longitude
  */
void idealGrid(int num, real** latitude, int* n_latitude,
               real** longitue, int* n_longitude);

#endif
