#include "function_management_service.h"

OBC_returnStateTypedef function_management_app(tc_tm_pkt *pkt) {

    uint32_t val;
    FM_fun_id fun_id;

    C_ASSERT(pkt != NULL && pkt->data != NULL) { return R_ERROR; }
    C_ASSERT(pkt->ser_subtype == TC_FM_PERFORM_FUNCTION) { return R_ERROR; }


    fun_id = pkt->data[0];
    cnv8_32(pkt->data[1], val);

    C_ASSERT(fun_id < LAST_FID) { return R_ERROR; }
    C_ASSERT(dev_id == OBC_SD)  { return R_ERROR; }

    ASSERT(pkt != NULL && pkt->data != NULL);

    if(fun_id == P_OFF || fun_id == P_ON || fun_id == P_RESET) { power_control_api(val, fun_id); } /*power management*/
    else if() /*time management*/

    return R_OK;
}