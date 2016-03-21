#ifndef __HOUSEKEEPING_SERVICE_H
#define __HOUSEKEEPING_SERVICE_H

#include <stdint.h>
#include "tc_tm.h"

extern uint32_t time_now();
extern OBC_returnStateTypedef route_pkt(tc_tm_pkt *pkt);
extern OBC_returnStateTypedef power_control_app_api(uint8_t did, uint8_t fid);

struct _obc_status {
    uint8_t mode;
    uint8_t batt_curr;
    uint8_t batt_volt;
    uint8_t bus_3v3_curr;
    uint8_t bus_5v_curr;
    uint8_t temp_eps;
    uint8_t temp_batt;
    uint8_t temp_comms;

}obc_status;

//ToDo
//  finish sid
//  when the get packet happens in crt pkt

OBC_returnStateTypedef hk_app(tc_tm_pkt *pkt);

OBC_returnStateTypedef hk_crt_pkt_TC(tc_tm_pkt *pkt, TC_TM_app_id app_id, uint8_t sid);

OBC_returnStateTypedef hk_crt_pkt_TM(tc_tm_pkt *pkt, TC_TM_app_id app_id, uint8_t sid);

void hk_SCH();

void clear_wod();

#endif
