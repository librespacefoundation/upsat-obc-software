#include "test_service.h"


#undef __FILE_ID__
#define __FILE_ID__ 9

SAT_returnState test_app(tc_tm_pkt *pkt) {

    tc_tm_pkt *temp_pkt = 0;

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true)       { return SATR_ERROR; }
    if(!C_ASSERT(pkt->ser_subtype == TC_CT_PERFORM_TEST) == true) { return SATR_ERROR; }

    test_crt_pkt(&temp_pkt, pkt->dest_id);
    if(!C_ASSERT(temp_pkt != NULL) == true) { return SATR_ERROR; }

    route_pkt(temp_pkt);
    return SATR_OK;
}

SAT_returnState test_crt_pkt(tc_tm_pkt **pkt, TC_TM_app_id dest_id) {

    *pkt = get_pkt();
    if(!C_ASSERT(*pkt != NULL) == true) { return SATR_ERROR; }
    crt_pkt(*pkt, OBC_APP_ID, TM, TC_ACK_NO, TC_TEST_SERVICE, TM_CT_REPORT_TEST, dest_id);

    (*pkt)->len = 0;

    return SATR_OK;
}