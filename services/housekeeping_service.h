#ifndef __HOUSEKEEPING_SERVICE_H
#define __HOUSEKEEPING_SERVICE_H

#include <stdint.h>
#include "services.h"

extern tc_tm_pkt * get_pkt();
extern SAT_returnState route_pkt(tc_tm_pkt *pkt);

extern SAT_returnState crt_pkt(tc_tm_pkt *pkt, TC_TM_app_id app_id, uint8_t type, uint8_t ack, uint8_t ser_type, uint8_t ser_subtype, TC_TM_app_id dest_id);

extern SAT_returnState hk_parameters_report(TC_TM_app_id app_id, HK_struct_id sid, uint8_t *data);
extern SAT_returnState hk_report_parameters(HK_struct_id sid, tc_tm_pkt *pkt);

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

#endif
