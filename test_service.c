#include "test_service.h"

OBC_returnStateTypedef test_app(tc_tm_pkt *pkt) {

    tc_tm_pkt *temp_pkt;

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true)     { return R_ERROR; }
    if(!C_ASSERT(pkt->subtype == TC_CT_PERFORM_TEST) == true)   { return R_ERROR; }

    test_crt_pkt(temp_pkt);
    route_pkt(temp_pkt);
    return R_OK;
}

OBC_returnStateTypedef test_crt_pkt(tc_tm_pkt *pkt) {

    pkt = get_pkt(NORMAL);
    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true) { return R_ERROR; }
    crt_pkt(pkt, OBC, TM, TC_ACK_NO, TC_TEST_SERVICE, TC_CT_REPORT_TEST, pkt->dest_id);

    pkt->len = 0;

    return R_OK;
}