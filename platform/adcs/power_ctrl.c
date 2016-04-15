#include "power_ctrl.h"


#undef __FILE_ID__
#define __FILE_ID__ 11

/*Must use real pins*/
SAT_returnState power_control_api(FM_dev_id did, FM_fun_id fid) {

    if(!C_ASSERT(did == ADCS_SD_DEV_ID) == true)                             { return SATR_ERROR; }
    if(!C_ASSERT(fid == P_OFF || fid == P_ON || fid == P_RESET) == true)    { return SATR_ERROR; }

    if(did == ADCS_SD_DEV_ID && fid == P_ON)         { HAL_adcs_SD_ON(); }
    else if(did == ADCS_SD_DEV_ID && fid == P_OFF)   { HAL_adcs_SD_OFF(); }
    else if(did == ADCS_SD_DEV_ID && fid == P_RESET) {
        HAL_adcs_SD_OFF();
        HAL_sys_delay(60);
        HAL_adcs_SD_ON();
    }

    return SATR_OK;
}