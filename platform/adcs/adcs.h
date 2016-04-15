#ifndef __ADCS_H
#define __ADCS_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../../../services/services.h"

struct _adcs_data
{
    uint16_t adcs_seq_cnt;
    uint8_t rsrc;
    uint32_t *boot_counter;
    uint32_t *log;
    uint32_t *log_cnt;
    uint32_t *log_state;

    struct uart_data obc_uart;
};

extern struct _adcs_data adcs_data;

extern const uint8_t services_verification_ADCS_TC[MAX_SERVICES][MAX_SUBTYPES];

extern SAT_returnState export_pkt(TC_TM_app_id app_id, tc_tm_pkt *pkt, struct uart_data *data);

extern uint32_t * HAL_obc_BKPSRAM_BASE();

extern SAT_returnState free_pkt(tc_tm_pkt *pkt);

extern SAT_returnState verification_app(tc_tm_pkt *pkt);
extern SAT_returnState hk_app(tc_tm_pkt *pkt);
extern SAT_returnState function_management_app(tc_tm_pkt *pkt);
extern SAT_returnState mass_storage_app(tc_tm_pkt *pkt);
extern SAT_returnState mass_storage_storeLogs(MS_sid sid, uint8_t *buf, uint16_t *size);
extern SAT_returnState large_data_app(tc_tm_pkt *pkt);
extern SAT_returnState test_app(tc_tm_pkt *pkt);

SAT_returnState route_pkt(tc_tm_pkt *pkt);

SAT_returnState event_log(uint8_t *buf, const uint16_t size);

SAT_returnState adcs_data_INIT();

#endif
