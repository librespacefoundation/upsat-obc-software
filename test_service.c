#include "test_service.h"

OBC_returnStateTypedef test_app(tc_tm_pkt *pkt) {

    test_crt_pkt(pkt);
    return R_OK;
}

OBC_returnStateTypedef test_crt_pkt(tc_tm_pkt *pkt) {

//    tc_tm_pkt *temp_pkt;

//    temp_pkt = get_pkt(NORMAL);
    crt_pkt(temp_pkt, OBC, TM, TC_ACK_NO, TC_TEST_SERVICE, TEST_REPORT, pkt->dest_id);
//    free_pkt(pkt);
    route_pkt(temp_pkt);
//    free_pkt(temp_pkt);

    return R_OK;
}