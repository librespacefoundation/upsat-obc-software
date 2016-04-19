#ifndef __EPS_H
#define __EPS_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../../../services/services.h"

//temp
#define TEST_ARRAY 1024

/*restriction for 8 char filename, for conversion from num to file name*/
#define MAX_FILE_NUM 0x5F5E0FF

struct _eps_data
{   
    struct uart_data dbg_uart;
    struct uart_data obc_uart;
};

extern struct _eps_data eps_data;

extern SAT_returnState export_pkt(TC_TM_app_id app_id, tc_tm_pkt *pkt, struct uart_data *data);

extern SAT_returnState free_pkt(tc_tm_pkt *pkt);

extern SAT_returnState verification_app(tc_tm_pkt *pkt);
extern SAT_returnState hk_app(tc_tm_pkt *pkt);
extern SAT_returnState function_management_app(tc_tm_pkt *pkt);
extern SAT_returnState mass_storage_app(tc_tm_pkt *pkt);
extern SAT_returnState mass_storage_storeLogs(MS_sid sid, uint8_t *buf, uint16_t *size);
extern SAT_returnState large_data_app(tc_tm_pkt *pkt);
extern SAT_returnState test_app(tc_tm_pkt *pkt);

SAT_returnState route_pkt(tc_tm_pkt *pkt);

#endif
