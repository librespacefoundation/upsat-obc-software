#ifndef MASS_STORAGE_SERVICE_H
#define MASS_STORAGE_SERVICE_H

#include <stdint.h>

#include "tc_tm.h"

#define MS_SU_LOG          "/SU_LOG"
#define MS_SU_SCRIPT_1     "/SU_SCR_1/SCR1.bin"
#define MS_SU_SCRIPT_2     "/SU_SCR_2/SCR2.bin"
#define MS_SU_SCRIPT_3     "/SU_SCR_3/SCR3.bin"
#define MS_SU_SCRIPT_4     "/SU_SCR_4/SCR4.bin"
#define MS_SU_SCRIPT_5     "/SU_SCR_5/SCR5.bin"
#define MS_SU_SCRIPT_6     "/SU_SCR_6/SCR6.bin"
#define MS_SU_SCRIPT_7     "/SU_SCR_7/SCR7.bin"
#define MS_EVENT_LOG       "/EVENT_LOG"
#define MS_FOTOS           "/FOTOS"

#define MS_TMP_SU_SCRIPT_1     "/SU_SCR_1/TMP_SCR1.bin"
#define MS_TMP_SU_SCRIPT_2     "/SU_SCR_2/TMP_SCR2.bin"
#define MS_TMP_SU_SCRIPT_3     "/SU_SCR_3/TMP_SCR3.bin"
#define MS_TMP_SU_SCRIPT_4     "/SU_SCR_4/TMP_SCR4.bin"
#define MS_TMP_SU_SCRIPT_5     "/SU_SCR_5/TMP_SCR5.bin"
#define MS_TMP_SU_SCRIPT_6     "/SU_SCR_6/TMP_SCR6.bin"
#define MS_TMP_SU_SCRIPT_7     "/SU_SCR_7/TMP_SCR7.bin"
#define MS_TMP_FOTOS           "/FOTOS/TMP"

#define MS_MAX_PATH             40 //random num
#define MS_MAX_FILES            30 //random num
#define MS_MAX_FNAME            30 //random num
#define MS_MAX_LOG_FILE_SIZE    512 //SD byte sectors 
#define MS_MAX_SU_FILE_SIZE     512 //SD byte sectors 
#define MS_FILE_SECTOR          512

typedef enum {  
SU_LOG              = 0,
SU_SCRIPT_1         = 1,
SU_SCRIPT_2         = 2,
SU_SCRIPT_3         = 3,
SU_SCRIPT_4         = 4,
SU_SCRIPT_5         = 5,
SU_SCRIPT_6         = 6,
SU_SCRIPT_7         = 7,
EVENT_LOG           = 8,
FOTOS               = 9,
TMP_SU_SCRIPT_1     = 10,
TMP_SU_SCRIPT_2     = 11,
TMP_SU_SCRIPT_3     = 12,
TMP_SU_SCRIPT_4     = 13,
TMP_SU_SCRIPT_5     = 14,
TMP_SU_SCRIPT_6     = 15,
TMP_SU_SCRIPT_7     = 16,
LAST_SID            = 17
}MS_sid;

typedef enum {  
ALL         = 0,
TO          = 1,
BETWEEN     = 2,
SPECIFIC    = 3,
LAST_PART   = 4,
LAST_MODE   = 5
}MS_mode;

struct _MS_data {
    FATFS test;
    uint16_t stores_size[MAX_SID];
    uint32_t ev_temp_log;
}MS_data;

//ToDo
//  error checking in return values of fatfs
//  check if sprintf is ok for the job
//  check various equations
//  assert, require.
//  finish definitions, types, subtypes, documentation and doc.
//  unit testing.
//  system testing.
//  check for EOF

OBC_returnStateTypedef mass_storage_init();

OBC_returnStateTypedef mass_storage_app(tc_tm_pkt *pkt);

OBC_returnStateTypedef mass_storage_delete_api(uint8_t sid, uint32_t to);

OBC_returnStateTypedef mass_storage_downlink_api(uint8_t sid, uint8_t mode, uint32_t from, uint32_t to, uint8_t *buf, uint16_t *size, uint16_t *part);

OBC_returnStateTypedef mass_storage_store_api(uint8_t sid, uint32_t *mode, uint8_t *buf, uint16_t *size, uint16_t *part);

OBC_returnStateTypedef mass_storage_report_api(uint8_t sid, uint8_t *buf, uint16_t *size, uint8_t *iter);


OBC_returnStateTypedef mass_storage_storeLargeFile(uint8_t sid, uint32_t *mode, uint8_t *buf, uint16_t *size, uint16_t *part);

OBC_returnStateTypedef mass_storage_storeLog(uint8_t sid, uint32_t *mode, uint8_t *buf, uint16_t *size);

OBC_returnStateTypedef mass_storage_downlinkLogs(uint8_t sid, uint8_t mode, uint32_t from, uint32_t to, uint8_t *buf, uint16_t *size, uint16_t *part);

OBC_returnStateTypedef mass_storage_downlinkLargeFile(uint8_t sid, uint8_t mode, uint32_t file, uint8_t *buf, uint16_t *size, uint16_t *part);

OBC_returnStateTypedef mass_storage_findLog(MS_sid sid, uint32_t *fn);

OBC_returnStateTypedef mass_storage_getFileName(uint8_t *fn);

#endif
