#ifndef __HOUSEKEEPING_SERVICE_H
#define __HOUSEKEEPING_SERVICE_H

#include <stdint.h>
#include "services.h"

extern uint32_t time_now();
extern tc_tm_pkt * get_pkt();
extern SAT_returnState route_pkt(tc_tm_pkt *pkt);
extern SAT_returnState power_control_api(FM_dev_id did, FM_fun_id fid);

extern SAT_returnState crt_pkt(tc_tm_pkt *pkt, TC_TM_app_id app_id, uint8_t type, uint8_t ack, uint8_t ser_type, uint8_t ser_subtype, TC_TM_app_id dest_id);
extern void cnv32_8(const uint32_t from, uint8_t *to);
extern void HAL_obc_delay(uint32_t sec);

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

//ToDo
//	verify sid reports
//  finish sid

//finished
//  sid to enum
//  when the get packet happens in crt pkt

SAT_returnState hk_app(tc_tm_pkt *pkt);

SAT_returnState hk_crt_pkt_TC(tc_tm_pkt *pkt, TC_TM_app_id app_id, HK_struct_id sid);

SAT_returnState hk_crt_pkt_TM(tc_tm_pkt *pkt, TC_TM_app_id app_id, HK_struct_id sid);

SAT_returnState hk_crt_empty_pkt_TM(tc_tm_pkt **pkt, TC_TM_app_id app_id, HK_struct_id sid);

void hk_SCH();

void clear_wod();

#endif
