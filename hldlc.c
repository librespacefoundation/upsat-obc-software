#include "hldlc.h"

/* it should return the status*/
OBC_returnStateTypedef HLDLC_deframe(uint8_t *buf, uint16_t *cnt, const uint8_t c) {

    if(!C_ASSERT(c != NULL && buf != NULL && buf != NULL) == true)  { return R_ERROR; }
    if(!C_ASSERT(cnt == 0 && c != HLDLC_START_FLAG) == true)        { return R_ERROR; } //error is when its true
    if(!C_ASSERT(cnt =< MAX_PKT) == true)                           { return R_ERROR; } //hard limits, check

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

    if(!C_ASSERT(c != NULL && buf != NULL && cnt != NULL)   { return R_ERROR; }
    if(!C_ASSERT(*cnt < size)                               { return R_ERROR; } //check

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
