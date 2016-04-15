#ifndef __OBC_H
#define __OBC_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../../../services/services.h"

//temp
#define TEST_ARRAY 1024

#define EV_MAX_BUFFER 1024

#define WOD_MAX_BUFFER 64

/*restriction for 8 char filename, for conversion from num to file name*/
#define MAX_FILE_NUM 0x5F5E0FF

struct _obc_data
{
    uint16_t obc_seq_cnt;
    uint32_t *file_id;
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

SAT_returnState obc_data_INIT();


void bkup_sram_INIT();

uint32_t get_new_fileId();


SAT_returnState event_log(uint8_t *buf, const uint16_t size);

SAT_returnState event_log_load(uint8_t *buf, const uint16_t pointer, const uint16_t size);

SAT_returnState event_log_IDLE();


#endif
