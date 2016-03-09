#ifndef MASS_STORAGE_SERVICE_H
#define MASS_STORAGE_SERVICE_H

#include <stdint.h>

#include "tc_tm.h"
#include "mass_storage_service.h"

#define MAX_LD_PKT_DATA MAX_EX_PKT_DATA - LD_PKT_HDR

struct _ld_status {
    uint8_t state;
    uint8_t sid;
    uint8_t app_id;
    uint8_t file;
    uint8_t last_ld_seq;
    uint32_t timeout;
} ld_status;

OBC_returnStateTypedef ld_SCH();

OBC_returnStateTypedef large_data_app(tc_tm_pkt *pkt);

OBC_returnStateTypedef large_data_startLog_api(uint8_t sid);

OBC_returnStateTypedef large_data_startLargeFile_api(uint8_t sid);

OBC_returnStateTypedef ld_rx_api(tc_tm_pkt *pkt);

OBC_returnStateTypedef ld_tx_api(tc_tm_pkt *pkt);

OBC_returnStateTypedef ld_crt_pkt(tc_tm_pkt *pkt);

#endif