#ifndef __POWER_CTRL_H
#define __POWER_CTRL_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

#include "tc_tm.h"
#include "verification_service.h"

uint8_t power_control_app(struct tc_tm_pkt *pkt);

uint8_t power_control_app_api(uint8_t did, uint8_t fid);

#endif