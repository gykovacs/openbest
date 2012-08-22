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

/**
  * represents the function "structureMerge" from the Matlab-BEST code (do not call directly!)
  * @param se stationElement2 object to make single valued
  * @returns new stationElement2 object
  */
stationElement2* structureMerge(stationElement2* se);

/**
  * any(ismember(flags, badFlags) Matlab construction
  * @param flags array of arrays of flags to check the membership
  * @param sizes of arrays in flags
  * @param size of flags
  * @param bf array of bad flags
  * @param number of bad flags
  * @returns indicator array of memberships
  */
flag_t* anyismember(flag_t** flags, char* n_flags, int n_n_flags, int* bf, int n);

/**
  * represents the function "mergeCore" in the Matlab-BEST code (do not call directly!)
  * @param se stationElement2 object to merge
  * @param bf bad flags
  * @param n number of bad flags
  * @param action mode of merging
  * @param result pointer to result stationElement2 object
  * @param number of data merged
  * @returns pointer of the merged result stationElement2 object
  */
stationElement2* mergeCore(stationElement2* se, int* bf, int n, char* action, stationElement2p* result, int** result_count);

/**
  * represents the function "maskOverFlags" in the Matlab-BEST code
  * @param flags array of arrays of flags
  * @param mask mask
  * @param size1 size of mask
  * @param c see Matlab code
  * @param l see Matlab code
  */
void maskOverFlags(flag_t** flags, int* mask, int size1, int c, int l);

/**
  * @param represents the function rangeResolver in the Matlab-BEST code
  * @param data input data series
  * @param unc input uncertainty serires
  * @param consitent see Matlab code
  * @param f index array - see Matlab code
  * @param n_f size of f
  * @param max_table see Matlab code
  * @param min_table see Matlab code
  * @param c see Matlab code
  * @param cc see Matlab code
  * @param k see Matlab code
  */
void rangeResolver(real* data, real* unc, int** consistent, int* f, int n_f, real* max_table, real* min_table, int c, int cc, int k);

/**
  * @param represents the function getIndexValue in the Matlab-BEST code
  * @param M see Matlab code
  * @param mask see Matlab code
  * @param size1 size of mask
  * @param c see Matlab code
  * @param v see Matlab code
  * @param est see Matlab code
  */
void getIndexValue(real* M, int* mask, int size1, int c, short* v, bool* est);

#endif
