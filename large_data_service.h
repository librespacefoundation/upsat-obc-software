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
    uint8_t ld_num;
    uint32_t timeout;
}LD_status;

//ToDo
//  assert, require.
//  finish definitions, types, subtypes, documentation and doc.

OBC_returnStateTypedef large_data_app(tc_tm_pkt *pkt);

OBC_returnStateTypedef large_data_startReport_api(uint8_t sid, tc_tm_pkt *pkt);

OBC_returnStateTypedef large_data_startDownlink_api(uint8_t sid, uint8_t mode, uint32_t from, uint32_t to, tc_tm_pkt *pkt);

#endif