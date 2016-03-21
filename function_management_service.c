#include "function_management_service.h"

OBC_returnStateTypedef function_management_app(tc_tm_pkt *pkt) {

    uint32_t val;
    FM_fun_id fun_id;

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true)             { return R_ERROR; }
    if(!C_ASSERT(pkt->ser_subtype == TC_FM_PERFORM_FUNCTION) == true)   { return R_ERROR; }


    fun_id = pkt->data[0];
    cnv8_32(pkt->data[1], val);

    if(!CC_ASSERT(fun_id < LAST_FUN_ID) == true)            { return R_ERROR; }
    if(!C_ASSERT(dev_id == OBC_SD_DEV_ID) == true)          { return R_ERROR; }
    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true) { return R_ERROR; }

    if(fun_id == P_OFF || fun_id == P_ON || fun_id == P_RESET) { power_control_api(val, fun_id); } /*power management*/
    //else if() /*time management*/

    return R_OK;
}
