#include "verification_service.h"

OBC_returnStateTypedef verify_pkt(tc_tm_pkt *pkt, uint8_t step, uint8_t res) {
/*this is TEMP*/
tc_tm_pkt out;

    ASSERT(res < LAST_CODE);
    ASSERT(pkt != NULL);
    ASSERT(pkt->data != NULL);

    REQUIRE(pkt->ack == TC_ACK_ALL || pkt->ack == TC_ACK_EXE_PROG || pkt->ack == TC_ACK_EXE_START));
    REQUIRE(pkt->type == TC); //check if this checks out

    if(pkt->ack == TC_ACK_NO) { return R_OK; } 
    else if(pkt->ack == TC_ACK_ACC && step == TC_ACK_ACC) {
        verify_crt_pkt(pkt, &out, res, 1);
        return R_OK;
    } else if(pkt->ack == TC_ACK_EXE_COMP && step == TC_ACK_EXE_COMP) {
        verify_crt_pkt(pkt, &out, res, 7);
        return R_OK; 
    }

    return R_ERROR;
}

OBC_returnStateTypedef verify_crt_pkt(tc_tm_pkt *pkt, tc_tm_pkt *out, uint8_t res, uint8_t subtype) {

    union _cnv cnv;

    ASSERT(pkt != NULL && pkt->data != NULL);
    ASSERT(res < LAST_CODE);

    out->type = TM;
    out->app_id = pkt->dest_id; 
    out->dest_id = pkt->app_id;

    cnv.cnv16[0] = pkt->app_id;
    out->data[0] = ( ECSS_VER_NUMBER << 5 | pkt->type << 4 | ECSS_DATA_FIELD_HDR_FLG << 3 | cnv.cnv8[1]);
    out->data[1] = cnv.cnv8[0];

    cnv.cnv16[0] = pkt->seq_count;
    out->data[2] = (pkt->seq_flags << 6 | cnv.cnv8[1]);
    out->data[3] = cnv.cnv8[0];

    out->ser_type = TC_VERIFICATION_SERVICE;

    if(res != R_OK) {
        out->data[4] = res;
        subtype++;
    }
    out->ser_subtype = subtype;

    return R_OK;
}

OBC_returnStateTypedef verification_pack_pkt_api(uint8_t *buf, tc_tm_pkt *pkt, uint16_t *buf_pointer) {

    ASSERT(pkt != NULL && buf != NULL && buf_pointer != NULL && pkt->data != NULL);

    buf[10] = pkt->data[0];
    buf[11] = pkt->data[1];
    buf[12] = pkt->data[2];
    buf[13] = pkt->data[3];

    if(pkt->ser_subtype == TC_TM_SER_TC_VER_ACC_SUCC || pkt->ser_subtype == TC_TM_SER_TC_VER_EXEC_START_SUCC || pkt->ser_subtype == TC_TM_SER_TC_VER_EXEC_COMP_SUCC ) {
        buf_pointer += 4;
    } else if(pkt->ser_subtype == TC_TM_SER_TC_VER_ACC_FAIL || pkt->ser_subtype == TC_TM_SER_TC_VER_EXEC_START_FAIL|| pkt->ser_subtype == TC_TM_SER_TC_VER_EXEC_COMP_FAIL) {
        buf[14] = pkt->data[4]; 
        buf_pointer += 5;
    } else { return R_ERROR; }

    return R_OK;
}