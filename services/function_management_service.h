#ifndef __FUNCTION_MANAGEMENT_SERVICE_H
#define __FUNCTION_MANAGEMENT_SERVICE_H

#include <stdint.h>
#include "services.h"

extern uint32_t time_now();
extern SAT_returnState power_control_api(FM_dev_id did, FM_fun_id fid);
extern void cnv8_32(uint8_t *from, uint32_t *to);

//ToDo
//  maybe make dev id = app id

SAT_returnState function_management_app(tc_tm_pkt *pkt);

#endif