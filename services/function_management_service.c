#include "function_management_service.h"


#undef __FILE_ID__
#define __FILE_ID__ 6

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
    else if(fun_id == SET_TIME) { /*time management*/

        uint8_t hours = 0;
        uint8_t mins = 0; 
        uint8_t sec = 0;

        //cnvQB50_UTC(val, &hours, &mins, &sec);
        HAL_obc_setTime(hours, mins, sec);
        //HAL_obc_setDate(hours, mins, sec);
    }

    return SATR_OK;
}

SAT_returnState function_management_pctrl_crt_pkt_api(tc_tm_pkt **pkt, TC_TM_app_id dest_id, FM_fun_id fun_id, FM_dev_id did) {

    *pkt = get_pkt();
    if(!C_ASSERT(*pkt != NULL) == true) { return SATR_ERROR; }
    crt_pkt(*pkt, OBC_APP_ID, TM, TC_ACK_NO, TC_TEST_SERVICE, TC_FM_PERFORM_FUNCTION, dest_id);

    (*pkt)->data[0] = fun_id;

    (*pkt)->data[1] = 0;
    (*pkt)->data[2] = 0;
    (*pkt)->data[3] = 0;
    (*pkt)->data[4] = did;

    (*pkt)->len = 5;

    return SATR_OK;
}
