#ifndef __FUNCTION_MANAGEMENT_SERVICE_H
#define __FUNCTION_MANAGEMENT_SERVICE_H

#include <stdint.h>
#include "services.h"

extern uint32_t time_now();
extern SAT_returnState power_control_api(FM_dev_id did, FM_fun_id fid);
extern void cnv8_32(uint8_t *from, uint32_t *to);
extern tc_tm_pkt * get_pkt(uint8_t mode);
extern SAT_returnState crt_pkt(tc_tm_pkt *pkt, TC_TM_app_id app_id, uint8_t type, uint8_t ack, uint8_t ser_type, uint8_t ser_subtype, TC_TM_app_id dest_id);

//ToDo
//	add timemanagement.
//  maybe make dev id = app id

SAT_returnState function_management_app(tc_tm_pkt *pkt);

SAT_returnState function_management_pctrl_crt_pkt_api(tc_tm_pkt **pkt, TC_TM_app_id dest_id, FM_fun_id fun_id, FM_dev_id did);

#endif