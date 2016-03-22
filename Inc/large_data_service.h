#ifndef MASS_STORAGE_SERVICE_H
#define MASS_STORAGE_SERVICE_H

#include <stdint.h>

#include "../Inc/services.h"
#include "../Inc/mass_storage_service.h"

#define MAX_LD_PKT_DATA MAX_EX_PKT_DATA - LD_PKT_HDR

extern SAT_returnState mass_storage_store_api(uint8_t sid, uint32_t *file, uint8_t *buf, uint16_t *size, uint16_t *part);
extern SAT_returnState mass_storage_move_api(uint8_t sid, uint32_t file_from, uint32_t file_to);

struct _ld_status {
    uint8_t state;
    uint8_t sid;
    uint8_t app_id;
    uint8_t file;
    uint8_t last_ld_seq;
    uint32_t timeout;
} ld_status;

SAT_returnState ld_SCH();

SAT_returnState large_data_app(tc_tm_pkt *pkt);

SAT_returnState ld_rx_api(tc_tm_pkt *pkt);

SAT_returnState ld_tx_api(tc_tm_pkt *pkt);

SAT_returnState ld_crt_pkt(tc_tm_pkt *pkt);

#endif