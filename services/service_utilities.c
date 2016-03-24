#include "service_utilities.h"

struct _obc_data obc_data;

// need to check endiannes
void cnv32_8(const uint32_t from, uint8_t *to) {

    union _cnv cnv;

    cnv.cnv32 = from;
    to[0] = cnv.cnv8[0];
    to[1] = cnv.cnv8[1];
    to[2] = cnv.cnv8[2];
    to[3] = cnv.cnv8[3];
}

void cnv16_8(const uint16_t from, uint8_t *to) {

    union _cnv cnv;

    cnv.cnv16[0] = from;
    to[0] = cnv.cnv8[0];
    to[1] = cnv.cnv8[1];

}

void cnv8_32(uint8_t *from, uint32_t *to) {

    union _cnv cnv;

    cnv.cnv8[0] = from[0];
    cnv.cnv8[1] = from[1];
    cnv.cnv8[2] = from[2];
    cnv.cnv8[3] = from[3];
    *to = cnv.cnv32;

}

void cnv8_16(uint8_t *from, uint16_t *to) {

    union _cnv cnv;

    cnv.cnv8[0] = from[0];
    cnv.cnv8[1] = from[1];
    *to = cnv.cnv16[0];
}

uint8_t checkSum(const uint8_t *data, uint16_t size) {

    uint8_t CRC = 0;

    if(!C_ASSERT(data != NULL && size != 0) == true) { return SATR_ERROR; }

    for(int i=0; i<=size; i++){
        CRC = CRC ^ data[i];
    }

    return CRC;
}

SAT_returnState route_pkt(tc_tm_pkt *pkt) {

    SAT_returnState res;
    uint16_t id;

//add error checking for pkt state
    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true)                         { free_pkt(pkt); return SATR_ERROR; }
    if(!C_ASSERT(pkt->type == TC || pkt->type == TM) == true)                       { free_pkt(pkt); return SATR_ERROR; }
    if(!C_ASSERT(pkt->app_id < LAST_APP_ID && pkt->dest_id < LAST_APP_ID) == true)  { free_pkt(pkt); return SATR_ERROR; }

    if(pkt->type == TC)         { id = pkt->app_id; } 
    else if(pkt->type == TM)    { id = pkt->dest_id; }

    if(id == OBC_APP_ID && pkt->ser_type == TC_HOUSEKEEPING_SERVICE) {
        //C_ASSERT(pkt->ser_subtype == 21 || pkt->ser_subtype == 23) { free_pkt(pkt); return SATR_ERROR; }
        res = hk_app(pkt);
        
    } else if(id == OBC_APP_ID && pkt->ser_type == TC_FUNCTION_MANAGEMENT_SERVICE) {
        res = function_management_app(pkt);
    } else if(id == OBC_APP_ID && pkt->ser_type == TC_LARGE_DATA_SERVICE) {
        res = large_data_app(pkt);
    } else if(id == OBC_APP_ID && pkt->ser_type == TC_MASS_STORAGE_SERVICE) {
        //C_ASSERT(pkt->ser_subtype == 1 || pkt->ser_subtype == 2 || pkt->ser_subtype == 9 || pkt->ser_subtype == 11 || pkt->ser_subtype == 12 || pkt->ser_subtype == 13) { free_pkt(pkt); return SATR_ERROR; }
        res = mass_storage_app(pkt);
    } else if(id == OBC_APP_ID && pkt->ser_type == TC_TEST_SERVICE) {
        //C_ASSERT(pkt->ser_subtype == 1 || pkt->ser_subtype == 2 || pkt->ser_subtype == 9 || pkt->ser_subtype == 11 || pkt->ser_subtype == 12 || pkt->ser_subtype == 13) { free_pkt(pkt); return SATR_ERROR; }
        res = test_app(pkt);
    } 
    else if(id == EPS_APP_ID)      { export_eps_pkt(pkt); } 
    else if(id == ADCS_APP_ID)     { export_eps_pkt(pkt); }
    else if(id == COMMS_APP_ID)    { export_eps_pkt(pkt); } 
    else if(id == IAC_APP_ID)      { export_eps_pkt(pkt); } 
    else if(id == GND_APP_ID)      { export_eps_pkt(pkt); } 

    verification_app(pkt);
    free_pkt(pkt);
    return SATR_OK;
}

SAT_returnState obc_data_INIT() {

    obc_data.obc_seq_cnt = 0;
    return SATR_OK;
}

SAT_returnState export_eps_pkt(tc_tm_pkt *pkt) {

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true) { return SATR_ERROR; }

    uint8_t c = 0;
    uint16_t size = 0;
    uint16_t cnt = 0;
    uint16_t cnt_out = 0;
    uint8_t buf[TEST_ARRAY];
    uint8_t buf_out[TEST_ARRAY];
    SAT_returnState res;    

    pack_pkt(buf, pkt, &size);
    for(uint16_t i = 0; i < size*2; i++) {
        res = HLDLC_frame(&c, buf, &cnt_out, size);
        if(res == SATR_EOT || res != SATR_ERROR) { cnt = i; break; }
        buf_out[i++] = c;   
    }

    if(!C_ASSERT(cnt > 0) == true) { return SATR_ERROR; }

    HAL_eps_uart_tx(buf_out, cnt);

    return SATR_OK;
}

SAT_returnState import_eps_pkt() {

    tc_tm_pkt *pkt;
    uint8_t c = 0;
    uint16_t cnt = 0;
    uint8_t buf[TEST_ARRAY];
    SAT_returnState res;    
    SAT_returnState res_deframe;

    res = HAL_eps_uart_rx(&c);
    if( res == SATR_OK ) {
        res_deframe = HLDLC_deframe(buf, &cnt, c);
        if(res_deframe == SATR_EOT) {
            
            //if(cnt > )
            pkt = get_pkt(NORMAL);
            if(!C_ASSERT(pkt != NULL) == true) { return SATR_ERROR; }
            if(unpack_pkt(buf, pkt, cnt) == SATR_OK) { route_pkt(pkt); } 
            else { verification_app(pkt); free_pkt(pkt); }
        }
    }

    return SATR_OK;
}

/*Must check for endianess*/
SAT_returnState unpack_pkt(const uint8_t *buf, tc_tm_pkt *pkt, const uint16_t size) {

    uint8_t tmp_crc[2];

    uint8_t ver, dfield_hdr, ccsds_sec_hdr, tc_pus;

    if(!C_ASSERT(buf != NULL && pkt != NULL && pkt->data != NULL) == true) { return SATR_ERROR; }

    tmp_crc[0] = buf[size - 1];
    tmp_crc[1] = checkSum(buf, size-2);

    ver = buf[0] >> 5;

    pkt->type = (buf[0] >> 4) & 0x01;
    dfield_hdr = (buf[0] >> 3) & 0x01;

    pkt->app_id = (TC_TM_app_id)buf[1];

    pkt->seq_flags = buf[2] >> 6;

    cnv8_16((uint8_t*)&buf[2], &pkt->seq_count);
    pkt->seq_count &= 0x3FFF;

    cnv8_16((uint8_t*)&buf[4], &pkt->len);

    ccsds_sec_hdr = buf[6] >> 7;

    tc_pus = buf[6] >> 4;

    pkt->ack = 0x04 & buf[6];

    pkt->ser_type = buf[7];
    pkt->ser_subtype = buf[8];
    pkt->dest_id = (TC_TM_app_id)buf[9];

    pkt->verification_state = SATR_ERROR;

    if(!C_ASSERT(pkt->app_id < LAST_APP_ID) == true) {
        pkt->verification_state = SATR_PKT_ILLEGAL_APPID;
        return SATR_PKT_ILLEGAL_APPID; 
    }

    if(!C_ASSERT(pkt->len == size - 7) == true) {
        pkt->verification_state = SATR_PKT_INV_LEN;
        return SATR_PKT_INV_LEN; 
    }
    pkt->len -= 4;

    if(!C_ASSERT(tmp_crc[0] == tmp_crc[1]) == true) {
        pkt->verification_state = SATR_PKT_INC_CRC;
        return SATR_PKT_INC_CRC; 
    }

    if(!C_ASSERT(services_verification_TC_TM[pkt->ser_type][pkt->ser_subtype][pkt->type] == 1) == true) { 
        pkt->verification_state = SATR_PKT_ILLEGAL_PKT_TP;
        return SATR_PKT_ILLEGAL_PKT_TP; 
    }

    if(!C_ASSERT(ver == ECSS_VER_NUMBER) == true) {
        pkt->verification_state = SATR_ERROR;
        return SATR_ERROR; 
    }

    if(!C_ASSERT(tc_pus == ECSS_PUS_VER) == true) {
        pkt->verification_state = SATR_ERROR;
        return SATR_ERROR;
    }

    if(!C_ASSERT(ccsds_sec_hdr == ECSS_SEC_HDR_FIELD_FLG) == true) {
        pkt->verification_state = SATR_ERROR;
        return SATR_ERROR;
    }

    if(!C_ASSERT(pkt->type == TC && pkt->type == TM) == true) {
        pkt->verification_state = SATR_ERROR;
        return SATR_ERROR;
    }

    if(!C_ASSERT(dfield_hdr == ECSS_DATA_FIELD_HDR_FLG) == true) {
        pkt->verification_state = SATR_ERROR;
        return SATR_ERROR;
    }

    if(!C_ASSERT(pkt->ack == TC_ACK_NO || pkt->ack == TC_ACK_ACC || pkt->ack == TC_ACK_EXE_COMP) == true) {
        pkt->verification_state = SATR_ERROR;
        return SATR_ERROR;
    }

    if(!C_ASSERT(pkt->seq_flags == TC_TM_SEQ_SPACKET) == true) {
        pkt->verification_state = SATR_ERROR;
        return SATR_ERROR;
    }

    for(int i = 0; i < pkt->len; i++) {
        pkt->data[i] = buf[10+i];
    }

    return SATR_OK;
}


/*buf: buffer to store the data to be sent, pkt: the data to be stored in the buffer, size: size of the array*/
SAT_returnState pack_pkt(uint8_t *buf, tc_tm_pkt *pkt, uint16_t *size) {

    union _cnv cnv;
    uint8_t buf_pointer;

    if(!C_ASSERT(buf != NULL && pkt != NULL && pkt->data != NULL  && size != NULL) == true) { return SATR_ERROR; }
    if(!C_ASSERT(*size != 0) == true)                                                       { return SATR_ERROR; }

    cnv.cnv16[0] = pkt->app_id;

    buf[0] = ( ECSS_VER_NUMBER << 5 | pkt->type << 4 | ECSS_DATA_FIELD_HDR_FLG << 3 | cnv.cnv8[1]);
    buf[1] = cnv.cnv8[0];

    /*if the pkt was created in OBC, it updates the counter*/
    if(pkt->app_id == OBC_APP_ID) { pkt->seq_count = obc_data.obc_seq_cnt++; }

    pkt->seq_flags = TC_TM_SEQ_SPACKET;
    cnv.cnv16[0] = pkt->seq_count;
    buf[2] = (  pkt->seq_flags << 6 | cnv.cnv8[1]);
    buf[3] = cnv.cnv8[0];

    /* TYPE = 0 TM, TYPE = 1 TC*/
    if(pkt->type == TM) {
        buf[6] = ECSS_PUS_VER << 4 ;
    } else if(pkt->type == TC) {
        buf[6] = ( ECSS_SEC_HDR_FIELD_FLG << 7 | ECSS_PUS_VER << 4 | pkt->ack);
    } else {
        return SATR_ERROR;
    }

    buf[7] = pkt->ser_type;
    buf[8] = pkt->ser_subtype;
    buf[9] = pkt->dest_id; /*source or destination*/

    buf_pointer = 10;

    for(int i = 0; i < pkt->len; i++) {
        buf[buf_pointer++] = pkt->data[i];
    }

    pkt->len += ECSS_DATA_HEADER_SIZE;

    /*check if this is correct*/
    cnv.cnv16[0] = buf_pointer - 6;
    buf[4] = cnv.cnv8[0];
    buf[5] = cnv.cnv8[1];

    /*added it for ecss conformity, checksum in the ecss is defined to have 16 bits, we only use 8*/
    buf[buf_pointer++] = 0;
    buf[buf_pointer] = checkSum(buf, buf_pointer-1);
    *size = buf_pointer;
    return SATR_OK;
}

SAT_returnState crt_pkt(tc_tm_pkt *pkt, TC_TM_app_id app_id, uint8_t type, uint8_t ack, uint8_t ser_type, uint8_t ser_subtype, TC_TM_app_id dest_id) {

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true)                 { return SATR_ERROR; }
    if(!C_ASSERT(app_id < LAST_APP_ID && dest_id < LAST_APP_ID ) == true)   { return SATR_ERROR; }
    if(!C_ASSERT(type == TC || type == TM) == true)                         { return SATR_ERROR; }
    if(!C_ASSERT(ack == TC_ACK_NO || ack == TC_ACK_ACC) == true)            { return SATR_ERROR; }

    pkt->type = type;
    pkt->app_id = app_id;
    pkt->dest_id = dest_id;

    pkt->ser_type = ser_type;
    pkt->ser_subtype = ser_subtype;

    return SATR_OK;
}

//WIP: will not compile
//should add a flag that writes the array in the sd in a idle task
//uint8 is temp and generic.
// SAT_returnState event_log(uint8_t *event) {
    
//     if(event+log_cnt > MAX EVENT FILE SIZE) {
//         mass_storage_storeLogs(EVENT_LOG, event_arr, uint16_t *size);
//     }

//     for(uint16_t i = 0; i < event size; ) {
//         event_arr[log_cnt++] = event[i];   
//     }
//     write2eeprom(event);

//     return SATR_OK;
// }
