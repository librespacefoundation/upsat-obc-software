#ifndef MASS_STORAGE_SERVICE_H
#define MASS_STORAGE_SERVICE_H

#include <stdint.h>

#include "../Inc/tc_tm.h"

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
#define SU_LOG

#define DELETE_ALL      0
#define DELETE_BEFORE   1

OBC_returnStateTypedef mass_storage_app(tc_tm_pkt *pkt);

OBC_returnStateTypedef mass_storage_mode_api(uint8_t sid, uint8_t mode);

OBC_returnStateTypedef mass_storage_delete_api(uint8_t sid, uint32_t to);

OBC_returnStateTypedef mass_storage_report_api(uint8_t sid, uint8_t *buf);

OBC_returnStateTypedef mass_storage_downlink_api(uint8_t sid, uint32_t file, uint8_t *buf, uint16_t *size, uint16_t *part);

OBC_returnStateTypedef mass_storage_store_api(uint8_t sid, uint32_t *file, uint8_t *buf, uint16_t *size, uint16_t *part);

OBC_returnStateTypedef mass_storage_move_api(uint8_t sid, uint32_t file_from, uint32_t file_to);

OBC_returnStateTypedef ms_crt_pkt_report(tc_tm_pkt *pkt, uint16_t app_id, uint8_t sid);

#endif
