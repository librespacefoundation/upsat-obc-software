#include "test_service.h"

SAT_returnState test_app(tc_tm_pkt *pkt) {

    tc_tm_pkt *temp_pkt = 0;

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true)       { return SATR_ERROR; }
    if(!C_ASSERT(pkt->ser_subtype == TC_CT_PERFORM_TEST) == true) { return SATR_ERROR; }

    test_crt_pkt(temp_pkt);
    if(!C_ASSERT(temp_pkt != NULL) == true) { return SATR_ERROR; }

    route_pkt(temp_pkt);
    return SATR_OK;
}

SAT_returnState test_crt_pkt(tc_tm_pkt *pkt) {

    pkt = get_pkt(NORMAL);
    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true) { return SATR_ERROR; }
    crt_pkt(pkt, OBC_APP_ID, TM, TC_ACK_NO, TC_TEST_SERVICE, TC_CT_REPORT_TEST, pkt->dest_id);

    pkt->len = 0;

    return SATR_OK;
}