#ifndef __POWER_CTRL_H
#define __POWER_CTRL_H

#include <stdint.h>
#include "../../../services/services.h"

extern void HAL_obc_SD_ON();
extern void HAL_obc_SD_OFF();
extern void HAL_obc_delay(uint32_t sec);
extern uint32_t time_now();

SAT_returnState power_control_api(FM_dev_id did, FM_fun_id fid);

#endif