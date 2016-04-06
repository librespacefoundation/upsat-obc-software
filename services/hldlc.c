#include "hldlc.h"

#include <stdio.h>

#undef __FILE_ID__
#define __FILE_ID__ 12

SAT_returnState HLDLC_deframe(uint8_t *buf, uint16_t *cnt, const uint8_t c, uint16_t *size) {

    if(!C_ASSERT(buf != 0) == true)                       { return SATR_ERROR; }
    if(!C_ASSERT(cnt != 0) == true)                       { return SATR_ERROR; }
    if(!C_ASSERT(*cnt == 0 && c == HLDLC_START_FLAG || *cnt > 0 ) == true)  { return SATR_ERROR; } /*the first char should be a start flag*/
    if(!C_ASSERT(*cnt <= TC_MAX_PKT_SIZE) == true)                          { return SATR_ERROR; } //hard limits, check

    if(*cnt != 0 && c == HLDLC_START_FLAG) {
        *size = *cnt;
        *cnt = 0;
        return SATR_EOT;
    } else if(cnt != 0 && buf[(*cnt)-1] == HLDLC_CONTROL_FLAG) {
        if(c == 0x5E) { buf[*cnt-1] = HLDLC_START_FLAG; }
        else if(c == 0x5D) { buf[*cnt-1] = HLDLC_CONTROL_FLAG; }
        else { return SATR_ERROR; }
        return SATR_OK;
    } else if(*cnt == 1 && buf[0] == HLDLC_START_FLAG) {
        buf[*cnt-1] = c;
        return SATR_OK;
    //} else if(*cnt == 0 && c == HLDLC_START_FLAG) {
    //    (*cnt)++;
    //    return SATR_OK;
    } else {
        buf[*cnt] = c;
        (*cnt)++;
        return SATR_OK;
    }
    return SATR_ERROR;
}

//used for DMA
SAT_returnState HLDLC_frame(uint8_t *buf_in, uint8_t *buf_out, uint16_t *size) {

    if(!C_ASSERT(buf_in != NULL && buf_out != NULL && size != NULL) == true)   { return SATR_ERROR; }
    printf("assertion is ok with size: %d\n", *size);
    uint16_t cnt = 2;

    for(uint16_t i = 0; i < *size; i++) {
        if(i == 0) {
            printf("assertion is ok with size: %d\n", *size);
            buf_out[0] = HLDLC_START_FLAG;
            buf_out[1] = buf_in[0];
        } else if(i == (*size) - 1) {
            buf_out[cnt++] = buf_in[i];
            buf_out[cnt] = HLDLC_START_FLAG;
            *size = cnt;
            return SATR_EOT;
        } else if(buf_in[i] == HLDLC_START_FLAG) {
            buf_out[cnt++] = HLDLC_CONTROL_FLAG;
            buf_out[cnt++] = 0x5E;
        } else if(buf_in[i] == HLDLC_CONTROL_FLAG) {
            buf_out[cnt++] = HLDLC_CONTROL_FLAG;
            buf_out[cnt++] = 0x5D;
        } else {
            buf_out[cnt++] = buf_in[i];
            //buf_out[++cnt] = buf_in[i];
        }

        printf("buf_out[0] = %u at %d iteration \n", buf_out[0], i);
    }

    return SATR_ERROR;
}
