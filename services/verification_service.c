#include "verification_service.h"


#undef __FILE_ID__
#define __FILE_ID__ 4

SAT_returnState verification_app(tc_tm_pkt *pkt) {

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true) { return SATR_ERROR; }

    if(!C_ASSERT(pkt->ack == TC_ACK_ACC || pkt->ack == TC_ACK_NO) == true) { return SATR_ERROR; } 
    if(pkt->type == TM) { return SATR_OK; } 

    if(pkt->ack == TC_ACK_NO) { return SATR_OK; } 
    else if(pkt->ack == TC_ACK_ACC) {

        tc_tm_pkt *temp_pkt = 0;

        verification_crt_pkt(pkt, &temp_pkt, pkt->verification_state);
        if(!C_ASSERT(temp_pkt != NULL) == true) { return SATR_ERROR; }

        route_pkt(temp_pkt);
    }

    return SATR_OK;
}

SAT_returnState verification_crt_pkt(tc_tm_pkt *pkt, tc_tm_pkt **out, SAT_returnState res) {

    uint8_t subtype;

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true) { return SATR_ERROR; }
    if(!C_ASSERT(res < SATR_LAST) == true)                  { return SATR_ERROR; }

    *out = get_pkt();
    if(!C_ASSERT(*out != NULL) == true) { return SATR_ERROR; }

    subtype = TM_VR_ACCEPTANCE_SUCCESS;

    (*out)->data[0] = (ECSS_VER_NUMBER << 5 | pkt->type << 4 | ECSS_DATA_FIELD_HDR_FLG << 3);
    (*out)->data[1] = (uint8_t)pkt->app_id;

    cnv16_8(pkt->seq_count, &(*out)->data[2]);
    (*out)->data[2] |= (pkt->seq_flags << 6 );

    (*out)->len = ECSS_VR_DATA_LEN_SUCCESS;

    if(res != SATR_OK) {
        (*out)->data[4] = (uint8_t)res;        /*failure reason*/
        subtype = TM_VR_ACCEPTANCE_FAILURE;
        (*out)->len = ECSS_VR_DATA_LEN_FAILURE;
    }

    crt_pkt(*out, SYSTEM_APP_ID, TM, TC_ACK_NO, TC_VERIFICATION_SERVICE, subtype, pkt->dest_id);

    return SATR_OK;
}
