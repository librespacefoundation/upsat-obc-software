#ifndef __ADCS_H
#define __ADCS_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define SYSTEM_APP_ID ADCS_APP_ID

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

SAT_returnState route_pkt(tc_tm_pkt *pkt);

#endif
