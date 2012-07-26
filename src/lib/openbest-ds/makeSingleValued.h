#include "openbest-ds/makeSingleValued.h"
#include "openbest-ds/stationElement2.h"
#include "openbest-ds/stationSite2.h"

/**
  * Takes a station element record that may be multi-valued and returns a single-valued alternative record. This operates on the assumption that all non-flagged values it is passed are equally acceptable, and expands the uncertainty to accommodate that if necessary.
  * @param se stationElement2 object pointer,
  * @param bf bad flags array
  * @param n length of bad flags array
  * @return new, single-valued stationElement2 object pointer
  */
stationElement2* makeSingleValued(stationElement2* se, int* bf, int n);

stationElement2* structureMerge(stationElement2* se);

flag_t* anyismember(flag_t** flags, char* n_flags, int n_n_flags, int* bf, int n);

stationElement2* mergeCore(stationElement2* se, int* bf, int n, char* action);

void maskOverFlags(flag_t* flags, int n_flags, int* mask, int n_mask, int width);

void rangeResolver(real* max_table, real* min_table, real* val, real* err, bool* consistent, int* n);

void getIndexValue(real* M, int n_M, int* mask, int n_mask);
