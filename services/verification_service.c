#include "verification_service.h"

SAT_returnState verification_app(tc_tm_pkt *pkt, SAT_returnState res) {

    if(!C_ASSERT(res < SATR_LAST) == true)                     { return SATR_ERROR; }
    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true) { return SATR_ERROR; }

    if(!C_ASSERT(pkt->ack == TC_ACK_ACC || pkt->ack == TC_ACK_NO || pkt->ack == TC_ACK_EXE_START || pkt->ack == TC_ACK_EXE_PROG || pkt->ack == TC_ACK_EXE_COMP || pkt->ack == TC_ACK_ALL) == true) { return SATR_ERROR; } 
    if(!C_ASSERT(pkt->type == TC) == true) { return SATR_ERROR; } 

    if(pkt->ack == TC_ACK_NO) { return SATR_OK; } 
    else if(pkt->ack == TC_ACK_ACC) {

        tc_tm_pkt *temp_pkt = 0;

        verification_crt_pkt(pkt, temp_pkt, res);
        if(!C_ASSERT(temp_pkt != NULL) == true) { return SATR_ERROR; }

        route_pkt(temp_pkt);
    } else if(pkt->ack == TC_ACK_EXE_START || pkt->ack == TC_ACK_EXE_PROG || pkt->ack == TC_ACK_EXE_COMP || pkt->ack == TC_ACK_ALL) {

        tc_tm_pkt *temp_pkt = 0;

        verification_crt_pkt(pkt, temp_pkt, SATR_PKT_ILLEGAL_ACK);
        if(!C_ASSERT(temp_pkt != NULL) == true) { return SATR_ERROR; }

        route_pkt(temp_pkt);
    }

    return SATR_OK;
}

SAT_returnState verification_crt_pkt(tc_tm_pkt *pkt, tc_tm_pkt *out, SAT_returnState res) {

    uint8_t subtype;

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true) { return SATR_ERROR; }
    if(!C_ASSERT(res < SATR_LAST) == true) { return SATR_ERROR; }

    out = get_pkt(NORMAL);
    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true) { return SATR_ERROR; }

    subtype = TC_VR_ACCEPTANCE_SUCCESS;

    out->data[0] = ( ECSS_VER_NUMBER << 5 | pkt->type << 4 | ECSS_DATA_FIELD_HDR_FLG << 3);
    out->data[1] = (uint8_t)pkt->app_id;

    cnv16_8(pkt->seq_count, &out->data[2]);
    out->data[2] |= (pkt->seq_flags << 6 );

    out->len = 4;

    if(res != SATR_OK) {
        out->data[4] = (uint8_t)res;        /*failure reason*/
        subtype = TC_VR_ACCEPTANCE_FAILURE;
        out->len = 5;
    }

    crt_pkt(out, pkt->dest_id, TM, TC_ACK_NO, TC_VERIFICATION_SERVICE, subtype, pkt->app_id);

    return SATR_OK;
}
