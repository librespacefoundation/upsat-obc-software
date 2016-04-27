#ifndef __POWER_CTRL_H
#define __POWER_CTRL_H

#include <stdint.h>
#include "../../../services/services.h"

extern void HAL_eps_OBC_ON();
extern void HAL_eps_OBC_OFF();
extern void HAL_eps_ADCS_ON();
extern void HAL_eps_ADCS_OFF();
extern void HAL_eps_COMMS_ON();
extern void HAL_eps_COMMS_OFF();
extern void HAL_eps_SU_ON();
extern void HAL_eps_SU_OFF();

extern void HAL_sys_delay(uint32_t sec);

SAT_returnState power_control_api(FM_dev_id did, FM_fun_id fid);

#endif
