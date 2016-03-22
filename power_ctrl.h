#ifndef __POWER_CTRL_H
#define __POWER_CTRL_H

#include <stdint.h>
#include "services.h"

extern void HAL_obc_SD_ON();
extern void HAL_obc_SD_OFF();
extern uint32_t time_now();

OBC_returnStateTypedef power_control_api(FM_dev_id did, FM_fun_id fid);

#endif