#ifndef __OBC_H
#define __OBC_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define SYSTEM_APP_ID OBC_APP_ID

struct _obc_data
{
    uint16_t obc_seq_cnt;
    uint8_t rsrc;
    uint32_t *file_id;
    uint32_t *boot_counter;
    uint32_t *log;
    uint32_t *log_cnt;
    uint32_t *log_state;
    uint32_t *wod_log;
    uint32_t *wod_cnt;

    struct uart_data dbg_uart;
    struct uart_data comms_uart;
    struct uart_data adcs_uart;
    struct uart_data eps_uart;
};

struct _sat_status {
    uint8_t mode;
    uint8_t batt_curr;
    uint8_t batt_volt;
    uint8_t bus_3v3_curr;
    uint8_t bus_5v_curr;
    uint8_t temp_eps;
    uint8_t temp_batt;
    uint8_t temp_comms;
};

extern struct _sat_status sat_status;

extern struct _obc_data obc_data;

extern const uint8_t services_verification_OBC_TC[MAX_SERVICES][MAX_SUBTYPES];

SAT_returnState route_pkt(tc_tm_pkt *pkt);

#endif
