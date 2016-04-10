#ifndef MASS_STORAGE_SERVICE_H
#define MASS_STORAGE_SERVICE_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "fatfs.h"
#include "services.h"

#define MS_SD_PATH "0:"

#define MS_SU_LOG          "/SU_LOG"
#define MS_WOD_LOG         "/WOD_LOG"
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
#define MS_MAX_SU_FILE_SIZE     2048 //2k
#define MS_FILE_SECTOR          512
#define MS_STORES               3
#define MS_SU_FSIZE             174
#define MS_MIN_SU_FILE 			1	//min is the header.

struct _MS_data {
    FATFS test;
    uint32_t stores_fsize[3];   /*total file size in each store, fotos, su_log, ev_log */
    uint16_t stores_fcount[3];  /*file count in each store, fotos, su_log, ev_log */
    //uint32_t ev_temp_log;
};

extern struct _MS_data MS_data;

extern SAT_returnState large_data_app(tc_tm_pkt *pkt);

extern uint32_t get_new_fileId();
extern void cnv8_32(uint8_t *from, uint32_t *to);
extern void cnv32_8(const uint32_t from, uint8_t *to);
extern void cnv8_16(uint8_t *from, uint16_t *to);

//ToDo
//  add format for sd
//	check type casting for snprintf & %d conversions
//	error checking for sprintf
//  strtol proper checking, return value range is ok
//  error checking in return values of fatfs
//  check various equations
//  assert, require.
//  finish definitions, documentation and doc.
//  check for EOF
//  maybe file paths should be const variable instead of definitions.

//Finished
//  add global counters for file and size, add check for array limits.
//  add check for MAX_FILE for loop, hard limit.
//  check if sprintf is ok for the job, used snprintf

SAT_returnState mass_storage_init();

SAT_returnState mass_storage_app(tc_tm_pkt *pkt);

SAT_returnState mass_storage_delete_api(MS_sid sid, uint32_t to);

SAT_returnState mass_storage_downlink_api(MS_sid sid, MS_mode mode, uint32_t from, uint32_t to, uint8_t *buf, uint16_t *size, uint32_t *part);

SAT_returnState mass_storage_store_api(MS_sid sid, MS_mode mode, uint8_t *buf, uint16_t *size, uint32_t part);

SAT_returnState mass_storage_report_api(MS_sid sid, uint8_t *buf, uint16_t *size, uint32_t *iter);


SAT_returnState mass_storage_su_checksum_api(MS_sid sid);

SAT_returnState mass_storage_su_load_api(MS_sid sid, uint8_t *buf);


SAT_returnState mass_storage_storeLargeFile(MS_sid sid, MS_mode mode, uint8_t *buf, uint16_t *size, uint32_t part);

SAT_returnState mass_storage_storeLogs(MS_sid sid, uint8_t *buf, uint16_t *size);

SAT_returnState mass_storage_downlinkLogs(MS_sid sid, MS_mode mode, uint32_t from, uint32_t to, uint8_t *buf, uint16_t *size, uint32_t *part);

SAT_returnState mass_storage_downlinkLargeFile(MS_sid sid, uint32_t file, uint8_t *buf, uint16_t *size, uint32_t *part);

SAT_returnState mass_storage_getLog(MS_sid sid, uint8_t *fn);

SAT_returnState mass_storage_findLog(MS_sid sid, uint32_t *fn);

SAT_returnState mass_storage_getFileName(uint8_t *fn);

SAT_returnState mass_storage_getFileSizeCount(MS_sid sid);

#endif
