#ifndef __HOUSEKEEPING_SERVICE_H
#define __HOUSEKEEPING_SERVICE_H

#include <stdint.h>
#include "services.h"

extern uint32_t time_now();
extern SAT_returnState route_pkt(tc_tm_pkt *pkt);
extern SAT_returnState power_control_api(FM_dev_id did, FM_fun_id fid);

extern SAT_returnState crt_pkt(tc_tm_pkt *pkt, uint16_t app_id, uint8_t type, uint8_t ack, uint8_t ser_type, uint8_t ser_subtype, uint16_t dest_id);
extern void cnv32_8(const uint32_t from, uint8_t *to);

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
//  sid to enum

SAT_returnState hk_app(tc_tm_pkt *pkt);

SAT_returnState hk_crt_pkt_TC(tc_tm_pkt *pkt, TC_TM_app_id app_id, uint8_t sid);

SAT_returnState hk_crt_pkt_TM(tc_tm_pkt *pkt, TC_TM_app_id app_id, uint8_t sid);

void hk_SCH();

void clear_wod();

#endif
