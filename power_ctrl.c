#include "power_ctrl.h"

/*Must use real pins*/
OBC_returnStateTypedef power_control_api( uint8_t did, uint8_t fid) {

    if(did == SD1 && fid == TURN_ON) {
        HAL_obc_SD_ON();
        return R_OK; 
    } else if(did == SD1 && fid == TURN_OFF) {
        HAL_obc_SD_OFF();
        return R_OK;
    } else if(did == SD1 && fid == RESET) {
        HAL_obc_SD_OFF();
        //delay
        HAL_obc_SD_ON();
        return R_OK;
    }

    return R_OK;
}