#ifndef _CONFIG_IO_H_
#define _CONFIG_IO_H_


/** data directory */
#define DATA_DIR "/mnt/gentoo/gsoc/openbest/dat/"

#define DATASET_DIR "multi-valued-tavg"

#define FILE_SEP "/"

#define FULL_SITE_DATA_FILE DATA_DIR FILE_SEP DATASET_DIR FILE_SEP "site_complete_detail.txt"

#define FULL_SITE_FLAGS_FILE DATA_DIR FILE_SEP DATASET_DIR FILE_SEP "site_flags.txt"

#define FULL_TEMP_DATA_FILE DATA_DIR FILE_SEP DATASET_DIR FILE_SEP "data.txt"

#define FULL_TEMP_FLAGS_FILE DATA_DIR FILE_SEP DATASET_DIR FILE_SEP "flags.txt"

#define FULL_SOURCES_FILE DATA_DIR FILE_SEP DATASET_DIR FILE_SEP "sources.txt"

#define FULL_STATION_CHANGE_FILE DATA_DIR FILE_SEP DATASET_DIR FILE_SEP "station_change.txt"


/** site data text file*/
#define SITE_DATA DATA_DIR FILE_SEP "site_complete_detail.txt.sample.1000.txt"
/** site flags text file*/
#define SITE_FLAGS DATA_DIR FILE_SEP "site_flags.txt.sample.1000.txt"

/** temperature data text file*/
#define TEMP_DATA DATA_DIR FILE_SEP "data.txt.sample.1000.txt"
/** temperature flags text file*/
#define TEMP_FLAGS DATA_DIR FILE_SEP "flags.txt.sample.1000.txt"

#define SOURCES_DATA DATA_DIR FILE_SEP "sources.txt.sample.1000.txt"

#define STATION_CHANGE DATA_DIR FILE_SEP "station_change.txt.sample.1000.txt"



#endif
