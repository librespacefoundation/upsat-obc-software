#include "verification_service.h"

OBC_returnStateTypedef verification_app(tc_tm_pkt *pkt, OBC_returnStateTypedef res) {

    if(!C_ASSERT(res < R_LAST) == true)                     { return R_ERROR; }
    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true) { return R_ERROR; }

    if(!C_ASSERT(pkt->ack == TC_ACK_ACC || pkt->ack == TC_ACK_NO || pkt->ack == TC_ACK_EXE_START || pkt->ack == TC_ACK_EXE_PROG || pkt->ack == TC_ACK_EXE_COMP || pkt->ack == TC_ACK_ALL) == true) { return R_ERROR; } 
    if(!C_ASSERT(pkt->type == TC) == true) { return R_ERROR; } 

    if(pkt->ack == TC_ACK_NO) { return R_OK; } 
    else if(pkt->ack == TC_ACK_ACC) {

        tc_tm_pkt *temp_pkt;

        verification_crt_pkt(pkt, temp_pkt, res);
        route_pkt(temp_pkt);
    } else if(pkt->ack == TC_ACK_EXE_START || pkt->ack == TC_ACK_EXE_PROG || pkt->ack == TC_ACK_EXE_COMP || pkt->ack == TC_ACK_ALL) {
        
        tc_tm_pkt *temp_pkt;

        verification_crt_pkt(pkt, temp_pkt, R_PKT_ILLEGAL_ACK);
        route_pkt(temp_pkt);
    }

    return R_OK;
}

OBC_returnStateTypedef verification_crt_pkt(tc_tm_pkt *pkt, tc_tm_pkt *out, OBC_returnStateTypedef res) {

    uint8_t subtype;
    union _cnv cnv;

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true) { return R_ERROR; }
    if(!C_ASSERT(res < R_LAST) == true) { return R_ERROR; }

    out = get_pkt(NORMAL);
    if(!C_ASSERT(out != NULL) == true) { return R_ERROR; }

    subtype = TC_VR_ACCEPTANCE_SUCCESS;

    cnv.cnv16[0] = pkt->app_id;
    out->data[0] = ( ECSS_VER_NUMBER << 5 | pkt->type << 4 | ECSS_DATA_FIELD_HDR_FLG << 3 | cnv.cnv8[1]);
    out->data[1] = cnv.cnv8[0];

    cnv.cnv16[0] = pkt->seq_count;
    out->data[2] = (pkt->seq_flags << 6 | cnv.cnv8[1]);
    out->data[3] = cnv.cnv8[0];

    out->len = 4;

    if(res != R_OK) {
        out->data[4] = res;
        subtype = TC_VR_ACCEPTANCE_FAILURE;
        out->len = 5;
    }

    crt_pkt(out, pkt->dest_id, TM, TC_ACK_NO, TC_VERIFICATION_SERVICE, subtype, pkt->app_id);

    return R_OK;
}
