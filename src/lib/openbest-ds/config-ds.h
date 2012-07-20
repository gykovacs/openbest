#ifndef _CONFIG_DS_H_
#define _CONFIG_DS_H_

#include <omp.h>

#define false 0
#define true 1

typedef double real;
typedef unsigned char bool;
//typedef unsigned short key_t;
typedef unsigned char flag_t;
typedef float temp_t;

char* unknown_value;

#define UNKNOWN_FLAG_STRING "unknown_value"

/** pointer to the string "unknown_value"*/
char* unknown_value;

#define COMPRESSION_AREA 500000

char* compression_area;

//extern const char* const dataDir = "/mnt/gentoo/openbest/dat/";
//#define DATA_DIR "/mnt/gentoo/gsoc/openbest/dat/"

#endif
