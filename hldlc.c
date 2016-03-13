#include "hldlc.h"

/* it should return the status*/
OBC_returnStateTypedef HLDLC_deframe(uint8_t *buf, uint16_t *cnt, const uint8_t c) {

    ASSERT(c != NULL && buf != NULL && buf != NULL);
    ASSERT(cnt == 0 && c != HLDLC_START_FLAG); //error is when its true
    ASSERT(cnt =< MAX_PKT); //hard limits, check

    if(*cnt != 0 && c == HLDLC_START_FLAG) {
        *cnt = 0;
        return R_EOT;
    } else if(cnt != 0 && buf[(*cnt)-1] == HLDLC_CONTROL_FLAG) {
        if(c == 0x5E) { buf[*cnt-1] = HLDLC_START_FLAG; }
        else if(c == 0x5D) { buf[*cnt-1] = HLDLC_CONTROL_FLAG; }
        else { return R_ERROR; }
        return R_OK;
    } else {
        buf[*cnt] = c;
        (*cnt)++;
        return R_OK;
    }
    return R_ERROR;
}

/* it should return the status*/
OBC_returnStateTypedef HLDLC_frame(uint8_t *c, uint8_t *buf, uint16_t *cnt, const uint16_t size) {

    ASSERT(c != NULL && buf != NULL && cnt != NULL);
    ASSERT(*cnt < size); //check

    if(*cnt == 0) {
        *c = HLDLC_START_FLAG;
        (*cnt)++;
        return R_OK;
    } else if(*cnt > size) {
        *c = HLDLC_START_FLAG;
        return R_EOT;
    } else if(cnt != 0 && buf[(*cnt)-1] == HLDLC_START_FLAG) {
        *c = HLDLC_CONTROL_FLAG;
        buf[(*cnt)-1] = 0x5E;
        return R_OK;
    } else if(cnt != 0 &&  buf[(*cnt)-1] == HLDLC_CONTROL_FLAG) {
        *c = HLDLC_CONTROL_FLAG;
        buf[(*cnt)-1] = 0x5D;
        return R_OK;
    } else {
        *c = buf[(*cnt)-1] ;
        (*cnt)++;
        return R_OK;
    }
    return R_ERROR;
}
