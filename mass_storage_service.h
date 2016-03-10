#ifndef MASS_STORAGE_SERVICE_H
#define MASS_STORAGE_SERVICE_H

#include <stdint.h>

#include "tc_tm.h"

#define SU_LOG
#define SU_SCRIPT_1
#define SU_SCRIPT_2
#define SU_SCRIPT_3
#define SU_SCRIPT_4
#define SU_SCRIPT_5
#define SU_SCRIPT_6
#define SU_SCRIPT_7
#define EVENT_LOG
#define FOTOS

#define DELETE_ALL      0
#define DELETE_BEFORE   1

struct _MS_data {
    FATFS test;
}MS_data;


OBC_returnStateTypedef mass_storage_init();

OBC_returnStateTypedef mass_storage_app(tc_tm_pkt *pkt);

OBC_returnStateTypedef mass_storage_mode_api(uint8_t sid, uint8_t mode);

OBC_returnStateTypedef mass_storage_delete_api(uint8_t sid, uint32_t to);

OBC_returnStateTypedef mass_storage_report_api(uint8_t sid, uint8_t *buf);

OBC_returnStateTypedef mass_storage_downlinkLargeFile_api(uint8_t sid, uint8_t mode, uint32_t from, uint32_t to, uint8_t *buf, uint16_t *size, uint16_t *part);

OBC_returnStateTypedef mass_storage_downlinkLog_api(uint8_t sid, uint8_t mode, uint32_t from, uint32_t to, uint8_t *buf, uint16_t *size, uint16_t *part);

OBC_returnStateTypedef mass_storage_storeLargeFile_api(uint8_t sid, uint32_t *file, uint8_t *buf, uint16_t *size, uint16_t *part);

OBC_returnStateTypedef mass_storage_storeLog_api(uint8_t sid, uint8_t *buf, uint16_t *size);

OBC_returnStateTypedef mass_storage_load_SU_Script(uint8_t *fn);

OBC_returnStateTypedef mass_storage_getFileName(uint8_t *fn);

OBC_returnStateTypedef mass_storage_getFileContents(uint8_t sid, uint32_t file, uint8_t *buf, uint16_t *size, uint16_t *part);


#endif
