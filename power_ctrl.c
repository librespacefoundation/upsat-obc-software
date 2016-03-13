#include "power_ctrl.h"

OBC_returnStateTypedef power_control_app(tc_tm_pkt *pkt) {

    uint8_t res, did, fid;

    ASSERT(pkt != NULL && pkt->data != NULL);

    did = pkt->data[0];
    fid = pkt->data[4];

    res = power_control_app_api( did, fid);
    verify_pkt(pkt, TC_ACK_ACC, res);
    return R_OK;
}

/*Must use real pins*/
OBC_returnStateTypedef power_control_app_api( uint8_t did, uint8_t fid) {

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
    } else if(did == OBC && fid == RESET) {
        return R_OK;
    } 

    return R_ERROR;
}