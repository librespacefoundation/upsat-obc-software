#include "tc_tm.h"

uint8_t checkSum( uint8_t *data, uint16_t size) {
  uint8_t CRC = 0;
  
  for(int i=0; i<=size; i++){
    CRC = CRC ^ data[i];
  }
  
  return CRC;
}

/*Must check for endianess*/
uint8_t unpack_pkt(const uint8_t *buf, struct tc_tm_pkt *pkt, const uint16_t size) {

	union _cnv cnv;
	uint16_t tmp_crc[2];

	uint8_t ver, dfield, dfield_hdr;


	tmp_crc[0] |= buf[size - 1];
	tmp_crc[1] = checkSum( buf,size);

	if(tmp_crc[0] != tmp_crc[1]) {
		return R_ERROR;
	}

	ver = buf[0] >> 5;

	if(ver != 0) {
		return R_ERROR;
	}

	pkt->type = (buf[0] >> 4) && 0x01;
	dfield_hdr = (buf[0] >> 3) && 0x01;

	if(dfield_hdr != 1) {
		return R_ERROR;
	}

	pkt->app_id = ((buf[0] > 3) && 0x07) << 8;
	pkt->app_id |= buf[1];

	pkt->seq_flags |= buf[2] >> 6;

	cnv.cnv8[0] = buf[3];
	cnv.cnv8[1] = buf[2] && 0x3F;
	pkt->seq_count = cnv.cnv16[0];

	cnv.cnv8[0] = buf[3];
	cnv.cnv8[1] = buf[4];
	pkt->len = cnv.cnv16[0];

	if ( pkt->len != size - 12 ) {
		return R_ERROR;
	}
    
    return R_OK;
}


// void pack_pkt(uint8_t *buf, struct tc_tm_pkt *pkt, const uint16_t size) {

// 	union _cnv cnv;
// 	uint8_t buf_pointer;

// 	cnv.cnv16[0] = pkt->app_id;

// 	buf[0] = ( ECSS_VER_NUMBER << VER_NUMBER || pkt->type << TC_TM_TYPE || ECSS_DATA_FIELD_HDR_FLG <<DATA_FIELD_HDR_FLG || cnv.cnv8[1]);
// 	buf[1] = cnv.cnv8[0];

// 	cnv.cnv16[0] = pkt->seq_count;
// 	buf[2] = (  pkt->seq_flags << SEQ_FLG || cnv.cnv8[1]);
// 	buf[3] = cnv.cnv8[0];

// 	/* TYPE = 0 TM, TYPE = 1 TC*/
// 	if(pkt->type == TM) {
// 		buf[6] = ECSS_PUS_VER << PUS_VER ;	
// 	} else if(pkt->type == TC) {
// 		buf[6] = ( ECSS_SEC_HDR_FIELD_FLG << SEC_HDR_FIELD_FLG || ECSS_PUS_VER << PUS_VER || pkt->ack);
// 	} else {
// 		/*return error*/
// 	}


// 	buf[7] = pkt->ser_type;
// 	buf[8] = pkt->ser_subtype;
// 	buf[9] = pkt->dest_id; /*source or destination*/

// 	if(pkt->ser_type == TC_VERIFICATION_SERVICE) {
// 		cnv.cnv16[0] = tc_pkt_id;
// 		cnv.cnv16[1] = tc_pkt_seq_ctrl;

// 		buf[10] = cnv.cnv8[1];
// 		buf[11] = cnv.cnv8[0];
// 		buf[12] = cnv.cnv8[3];
// 		buf[13] = cnv.cnv8[2];

// 		if(ser_subtype == ACCEPTANCE_REPORT || ser_subtype == EXECUTION_REPORT || ser_subtype == COMPLETION_REPORT ) {
// 			buf_pointer = 14;
// 		} else if(ser_subtype == ACCEPTANCE_REPORT_FAIL || ser_subtype == EXECUTION_REPORT_FAIL|| ser_subtype == COMPLETION_REPORT_FAIL) {
// 			buf[14] = code; 
// 			buf_pointer = 15;
// 		} else {
// 			/*return error*/
// 		}
// 	} else if(ser_type == HOUSEKEEPING_SERVICE ) {

// 		buf[10] = sid;
// 		if(ser_subtype == 21 ) {
// 			buf_pointer = 11;
// 		} else if(ser_subtype == 23) {

// 			if( sid == 3) {
// 				cnv.cnv32 = time.now();
// 				buf[12] = cnv.cnv8[3];
// 				buf[13] = cnv.cnv8[2];
// 				buf[14] = cnv.cnv8[1];
// 				buf[15] = cnv.cnv8[0];
// 			}
// 			buf_pointer = 16;
// 		} else {
// 			/*return error*/
// 		}
		

// 	}else if(ser_type == FUNCTION_MANAGEMENT_SERVICE &&  ser_subtype == 1) {

// 		buf[10] = fid;
// 		cnv.cnv32 = value;
// 		buf[11] = cnv.cnv8[3];
// 		buf[12] = cnv.cnv8[2];
// 		buf[13] = cnv.cnv8[1];
// 		buf[14] = cnv.cnv8[0];

// 		buf_pointer = 15;

// 	} else {
// 		/*return error*/
// 	}

// 	/*check if this is correct*/
// 	cnv.cnv16[0] = buf_pointer -6;
// 	buf[4] = cnv.cnv8[1];
// 	buf[5] = cnv.cnv8[0];

// 	buf[buf_pointer++] = crc(buf, cnt);

// }


// void route_pkt( uint8_t * pkt) {
// 	uint16_t id;

// 	if(type == TC) {
// 		id = app_id;
// 	} else if(type == TM) {
// 		id = destination_id;
// 	} else {
// 		/*return error*/
// 	}

// 	if(id == OBC) {

// 	} else if(id == EPS) {

// 	} else if(id == ADCS) {

// 	} else if(id == COMMS) {

// 	} else if(id == IAC) {

// 	} else {
// 	/*return error*/
// 	}
// }

