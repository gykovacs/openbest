#ifndef _MAKE_SINGLE_VALUED_H_
#define _MAKE_SINGLE_VALUED_H_

#include "openbest-ds/config-ds.h"
#include "openbest-ds/primaryKeyTables.h"
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

stationElement2* mergeCore(stationElement2* se, int* bf, int n, char* action, stationElement2p* result, int** result_count);

void maskOverFlags(flag_t** flags, int* mask, int size1, int c, int l);

void rangeResolver(real* data, real* unc, int** consistent, int* f, int n_f, real* max_table, real* min_table, int c, int cc, int k);

void getIndexValue(real* M, int* mask, int size1, int c, short* v, bool* est);

#endif
