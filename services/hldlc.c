#include "hldlc.h"

SAT_returnState HLDLC_deframe(uint8_t *buf, uint16_t *cnt, const uint8_t c) {

    if(!C_ASSERT(buf != NULL && buf != NULL) == true)                       { return SATR_ERROR; }
    if(!C_ASSERT(*cnt == 0 && c == HLDLC_START_FLAG || *cnt > 0 ) == true)  { return SATR_ERROR; } /*the first char should be a start flag*/
    if(!C_ASSERT(*cnt <= TC_MAX_PKT_SIZE) == true)                          { return SATR_ERROR; } //hard limits, check

    if(*cnt != 0 && c == HLDLC_START_FLAG) {
        *cnt = 0;
        return SATR_EOT;
    } else if(cnt != 0 && buf[(*cnt)-1] == HLDLC_CONTROL_FLAG) {
        if(c == 0x5E) { buf[*cnt-1] = HLDLC_START_FLAG; }
        else if(c == 0x5D) { buf[*cnt-1] = HLDLC_CONTROL_FLAG; }
        else { return SATR_ERROR; }
        return SATR_OK;
    } else {
        buf[*cnt] = c;
        (*cnt)++;
        return SATR_OK;
    }
    return SATR_ERROR;
}

SAT_returnState HLDLC_frame(uint8_t *c, uint8_t *buf, uint16_t *cnt, const uint16_t size) {

    if(!C_ASSERT(c != NULL && buf != NULL && cnt != NULL) == true)   { return SATR_ERROR; }
    if(!C_ASSERT(*cnt < size) == true)                               { return SATR_ERROR; } //check

    if(*cnt == 0) {
        *c = HLDLC_START_FLAG;
        (*cnt)++;
        return SATR_OK;
    } else if(*cnt > size) {
        *c = HLDLC_START_FLAG;
        return SATR_EOT;
    } else if(cnt != 0 && buf[(*cnt)-1] == HLDLC_START_FLAG) {
        *c = HLDLC_CONTROL_FLAG;
        buf[(*cnt)-1] = 0x5E;
        return SATR_OK;
    } else if(cnt != 0 &&  buf[(*cnt)-1] == HLDLC_CONTROL_FLAG) {
        *c = HLDLC_CONTROL_FLAG;
        buf[(*cnt)-1] = 0x5D;
        return SATR_OK;
    } else {
        *c = buf[(*cnt)-1] ;
        (*cnt)++;
        return SATR_OK;
    }
    return SATR_ERROR;
}
