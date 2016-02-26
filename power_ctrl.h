#ifndef __POWER_CTRL_H
#define __POWER_CTRL_H

#include <stdint.h>

#include "tc_tm.h"
#include "verification_service.h"

extern void HAL_obc_SD_ON();

extern void HAL_obc_SD_OFF();

OBC_returnStateTypedef power_control_app(struct tc_tm_pkt *pkt);

OBC_returnStateTypedef power_control_app_api(uint8_t did, uint8_t fid);

#endif