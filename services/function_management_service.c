#include "function_management_service.h"

SAT_returnState function_management_app(tc_tm_pkt *pkt) {

    uint32_t val;
    FM_fun_id fun_id;

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true)             { return SATR_ERROR; }
    if(!C_ASSERT(pkt->ser_subtype == TC_FM_PERFORM_FUNCTION) == true)   { return SATR_ERROR; }


    fun_id = (FM_fun_id)pkt->data[0];
    cnv8_32(&pkt->data[1], &val);

    if(!C_ASSERT(fun_id < LAST_FUN_ID) == true)             { return SATR_ERROR; }
    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true) { return SATR_ERROR; }

    if(fun_id == P_OFF || fun_id == P_ON || fun_id == P_RESET) {  /*power management*/
        pkt->verification_state = SATR_OK; 
        power_control_api((FM_dev_id)val, fun_id); 
    }
    //else if() /*time management*/

    return SATR_OK;
}
