#include "power_ctrl.h"

#undef __FILE_ID__
#define __FILE_ID__ 11

/*Must use real pins*/
SAT_returnState power_control_api(FM_dev_id did, FM_fun_id fid) {

    if(!C_ASSERT(did == OBC_DEV_ID || did == ADCS_DEV_ID || did == COMMS_DEV_ID || did == SU_DEV_ID) == true)                             { return SATR_ERROR; }
    if(!C_ASSERT(fid == P_OFF || fid == P_ON || fid == P_RESET) == true)    { return SATR_ERROR; }

    if(did == OBC_DEV_ID && fid == P_ON)         { HAL_eps_OBC_ON(); }
    else if(did == OBC_DEV_ID && fid == P_OFF)   { HAL_eps_OBC_OFF(); }
    else if(did == OBC_DEV_ID && fid == P_RESET) {
        HAL_eps_OBC_OFF();
        HAL_sys_delay(60);
        HAL_eps_OBC_ON();
    } else if(did == ADCS_DEV_ID && fid == P_ON)  { HAL_eps_ADCS_ON(); }
    else if(did == ADCS_DEV_ID && fid == P_OFF)   { HAL_eps_ADCS_OFF(); }
    else if(did == ADCS_DEV_ID && fid == P_RESET) {
        HAL_eps_ADCS_OFF();
        HAL_sys_delay(60);
        HAL_eps_ADCS_ON();
    } else if(did == COMMS_DEV_ID && fid == P_ON)  { HAL_eps_COMMS_ON(); }
    else if(did == COMMS_DEV_ID && fid == P_OFF)   { HAL_eps_COMMS_OFF(); }
    else if(did == COMMS_DEV_ID && fid == P_RESET) {
        HAL_eps_COMMS_OFF();
        HAL_sys_delay(60);
        HAL_eps_COMMS_ON();
    } else if(did == SU_DEV_ID && fid == P_ON)  { HAL_eps_SU_ON(); }
    else if(did == SU_DEV_ID && fid == P_OFF)   { HAL_eps_SU_OFF(); }
    else if(did == SU_DEV_ID && fid == P_RESET) {
        HAL_eps_SU_OFF();
        HAL_sys_delay(60);
        HAL_eps_SU_ON();
    }

    return SATR_OK;
}
