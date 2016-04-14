#include "housekeeping_service.h"

#undef __FILE_ID__
#define __FILE_ID__ 5

SAT_returnState hk_app(tc_tm_pkt *pkt) {

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true) { return SATR_ERROR; }
    if(!C_ASSERT(pkt->ser_subtype == TC_HK_REPORT_PARAMETERS || pkt->ser_subtype == TM_HK_PARAMETERS_REPORT) == true) { return SATR_ERROR; }

    if(pkt->ser_subtype == TC_HK_REPORT_PARAMETERS) {

        tc_tm_pkt *temp_pkt = 0;
        HK_struct_id sid = (HK_struct_id)pkt->data[0];

        hk_crt_empty_pkt_TM(&temp_pkt, (TC_TM_app_id)pkt->dest_id, sid);
        if(!C_ASSERT(temp_pkt != NULL) == true) { return SATR_ERROR; }

        route_pkt(temp_pkt);

    } else if(pkt->app_id == EPS_APP_ID && pkt->ser_subtype == TM_HK_PARAMETERS_REPORT) {

        SAT_returnState res = hk_parameters_report(pkt->app_id, (HK_struct_id)pkt->data[0],  pkt->data);

        if(res == SATR_OK) { pkt->verification_state = SATR_OK; }
    }

    return SATR_OK;
}

SAT_returnState hk_crt_empty_pkt_TM(tc_tm_pkt **pkt, TC_TM_app_id app_id, HK_struct_id sid) {

    *pkt = get_pkt();
    if(!C_ASSERT(*pkt != NULL) == true) { return SATR_ERROR; }

    hk_crt_pkt_TM(*pkt, app_id, sid);
    return SATR_OK;
}

SAT_returnState hk_crt_pkt_TC(tc_tm_pkt *pkt, TC_TM_app_id app_id, HK_struct_id sid) {

    if(!C_ASSERT(app_id < LAST_APP_ID) == true)  { return SATR_ERROR; }

    crt_pkt(pkt, app_id, TC, TC_ACK_NO, TC_HOUSEKEEPING_SERVICE, TC_HK_REPORT_PARAMETERS, SYSTEM_APP_ID);

    pkt->data[0] = (char)sid;
    pkt->len = 1;

    return SATR_OK;
}

SAT_returnState hk_crt_pkt_TM(tc_tm_pkt *pkt, TC_TM_app_id app_id, HK_struct_id sid) {

    hk_report_parameters(sid, pkt);

    crt_pkt(pkt, SYSTEM_APP_ID, TM, TC_ACK_NO, TC_HOUSEKEEPING_SERVICE, TM_HK_PARAMETERS_REPORT, app_id);

    return SATR_OK;
}
