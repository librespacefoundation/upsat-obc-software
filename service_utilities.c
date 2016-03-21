#include "service_utilities.h"

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

    cnv.cnv16 = from;
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
    *to = cnv.cnv16;
}

uint8_t checkSum(const uint8_t *data, uint16_t size) {

    uint8_t CRC = 0;

    ASSERT(data != NULL && size != 0);

    for(int i=0; i<=size; i++){
        CRC = CRC ^ data[i];
    }

    return CRC;
}

/*Must check for endianess*/
OBC_returnStateTypedef unpack_pkt(const uint8_t *buf, tc_tm_pkt *pkt, const uint16_t size) {

    union _cnv cnv;
    uint8_t tmp_crc[2];

    uint8_t ver, dfield_hdr, ccsds_sec_hdr, tc_pus;

    if(!C_ASSERT(buf != NULL && pkt != NULL && pkt->data != NULL) == true) { return R_ERROR; }

    tmp_crc[0] = buf[size - 1];
    tmp_crc[1] = checkSum(buf, size-2);

    ver = buf[0] >> 5;

    pkt->type = (buf[0] >> 4) & 0x01;
    dfield_hdr = (buf[0] >> 3) & 0x01;

    cnv.cnv8[0] = buf[1];
    cnv.cnv8[1] = 0x07 & buf[0];
    pkt->app_id = cnv.cnv16[0];

    pkt->seq_flags = buf[2] >> 6;

    cnv.cnv8[0] = buf[3];
    cnv.cnv8[1] = buf[2] & 0x3F;
    pkt->seq_count = cnv.cnv16[0];

    cnv.cnv8[0] = buf[4];
    cnv.cnv8[1] = buf[5];
    pkt->len = cnv.cnv16[0];

    ccsds_sec_hdr = buf[6] >> 7;

    tc_pus = buf[6] >> 4;

    pkt->ack = 0x04 & buf[6];

    pkt->ser_type = buf[7];
    pkt->ser_subtype = buf[8];
    pkt->dest_id = buf[9];

    REQUIRE(app_id_verification[pkt->app_id] != 1) { 
        return R_PKT_ILLEGAL_APPID; 
    }

    REQUIRE(pkt->len != size - 7) { 
        return R_PKT_INV_LEN; 
    }
    pkt->len -= 4;

    REQUIRE(tmp_crc[0] != tmp_crc[1]) { 
        return R_PKT_INC_CRC; 
    }

    REQUIRE(services_verification_TC_TM[pkt->ser_type][pkt->ser_subtype][pkt->type] != 1) { 
        return R_PKT_ILLEGAL_PKT_TP; 
    }

    REQUIRE(ver != 0) { 
        return R_ERROR; 
    }

    REQUIRE(tc_pus != 1) {
        return R_ERROR;
    }

    REQUIRE(ccsds_sec_hdr != 0) {
        return R_ERROR;
    }

    REQUIRE(pkt->type != TC && pkt->type != TM) {
        return R_ERROR;
    }

    REQUIRE(dfield_hdr != 1) {
        return R_ERROR;
    }

    REQUIRE(pkt->ack != TC_ACK_NO || pkt->ack != TC_ACK_ACC || pkt->ack != TC_ACK_EXE_COMP) {
        return R_ERROR;
    }

    for(int i = 0; i < pkt->len; i++) {
        pkt->data[i] = buf[10+i];
    }

    return R_OK;
}


/*buf: buffer to store the data to be sent, pkt: the data to be stored in the buffer, size: size of the array*/
OBC_returnStateTypedef pack_pkt(uint8_t *buf, tc_tm_pkt *pkt, uint16_t *size) {

    union _cnv cnv;
    uint8_t buf_pointer;

    ASSERT(buf != NULL && pkt != NULL && pkt->data != NULL  && size != NULL);
    REQUIRE(*size == 0);

    cnv.cnv16[0] = pkt->app_id;

    buf[0] = ( ECSS_VER_NUMBER << 5 | pkt->type << 4 | ECSS_DATA_FIELD_HDR_FLG << 3 | cnv.cnv8[1]);
    buf[1] = cnv.cnv8[0];

    cnv.cnv16[0] = pkt->seq_count;
    buf[2] = (  pkt->seq_flags << 6 | cnv.cnv8[1]);
    buf[3] = cnv.cnv8[0];

    /* TYPE = 0 TM, TYPE = 1 TC*/
    if(pkt->type == TM) {
        buf[6] = ECSS_PUS_VER << 4 ;
    } else if(pkt->type == TC) {
        buf[6] = ( ECSS_SEC_HDR_FIELD_FLG << 7 | ECSS_PUS_VER << 4 | pkt->ack);
    } else {
        return R_ERROR;
    }

    buf[7] = pkt->ser_type;
    buf[8] = pkt->ser_subtype;
    buf[9] = pkt->dest_id; /*source or destination*/

    buf_pointer = 10;

    for(int i = 0; i < pkt->len; i++) {
        buf[buf_pointer++] = pkt-pkt->data[i];
    }

    pkt->len += ECSS_DATA_HEADER_SIZE;

    /*check if this is correct*/
    cnv.cnv16[0] = buf_pointer - 6;
    buf[4] = cnv.cnv8[0];
    buf[5] = cnv.cnv8[1];

    buf[buf_pointer] = checkSum(buf, buf_pointer-1);
    *size = buf_pointer;
    return R_OK;
}

OBC_returnStateTypedef crt_pkt(tc_tm_pkt *pkt, uint16_t app_id, uint8_t type, uint8_t ack, uint8_t ser_type, uint8_t ser_subtype, uint16_t dest_id) {

    C_ASSERT(pkt != NULL && pkt->data != NULL) { return R_ERROR; }
    C_ASSERT(app_id < LAST_APP_ID && dest_id < LAST_APP_ID ) { return R_ERROR; }
    C_ASSERT(type == TC || type == TM) { return R_ERROR; }
    C_ASSERT(ack == TC_ACK_NO || ack == TC_ACK_ACC) { return R_ERROR; }

    pkt->type = type;
    pkt->app_id = app_id;
    pkt->dest_id = dest_id;

    pkt->ser_type = ser_type;
    pkt->ser_subtype = ser_subtype;

    return R_OK;
}

