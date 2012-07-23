#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <omp.h>

/** definition of logical false*/
#define false 0
/** definition of logical true*/
#define true 1

/** definition of real type*/
typedef double real;
/** definition of boolean type*/
typedef unsigned char bool;
//typedef unsigned short key_t;
/** definition of flag type*/
typedef unsigned char flag_t;
/** definition of temperature data type*/
typedef float temp_t;

#define UNKNOWN_FLAG_STRING "unknown_value"

/** pointer to the string "unknown_value"*/
char* unknown_value;

#define COMPRESSION_AREA 500000

char* compression_area;

//extern const char* const dataDir = "/mnt/gentoo/openbest/dat/";
//#define DATA_DIR "/mnt/gentoo/gsoc/openbest/dat/"

#endif
