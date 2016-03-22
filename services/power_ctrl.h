#ifndef __POWER_CTRL_H
#define __POWER_CTRL_H

#include <stdint.h>

#include "../Inc/services.h"
#include "../Inc/verification_service.h"

extern void HAL_obc_SD_ON();

extern void HAL_obc_SD_OFF();

SAT_returnState power_control_app(tc_tm_pkt *pkt);

SAT_returnState power_control_app_api(uint8_t did, uint8_t fid);

#endif