#include tc_tm.h


/*Must check for endianess*/
void unpack_pkt(const uint8_t *buf, const uint16_t size) {

	uint16_t tmp_crc[2];

	uint8_t ver,type, dfield;
	uint16_t app_id, seq_cnt, pkt_len;

	tmp_crc[0] = buf[size - 1];
	tmp_crc[0] |= buf[size - 2] << 8;
	tmp_crc[1] = crc( buf,size);

	if(tmp_crc[0] != tmp_crc[1]) {
		/*return crc error*/
	}

	ver = buf[0] >> 5;

	if(ver != 0) {
		/*return error*/
	}

	type = (buf[0] >> 4) && 0x01;
	dfield_hdr = (buf[0] >> 3) && 0x01;

	if(dfield_hdr != 1) {
		/*return error*/
	}

	app_id = ((buf[0] > 3) && 0x07) << 8;
	app_id |= buf[1];

	seq_flg |= buf[2] >> 6;

	seq_cnt = (buf[2] && 0x3F) << 8;
	seq_cnt |= buf[3];

	pkt_len = (buf[4] << 8 ) || buf[5];
	pkt_len = (buf[4] << 8 ) || buf[5];

	if ( pkt_len != size - 12 ) {
		/*return error*/
	}




}


void pack_pkt(uint8_t *buf, const uint16_t size) {

	union _cnv cnv;
	uint8_t buf_pointer;

	cnv.cnv16[0] = app_id;

	buf[0] = ( ECSS_VER_NUMBER << VER_NUMBER || type << TC_TM_TYPE || ECSS_DATA_FIELD_HDR_FLG << DATA_FIELD_HDR_FLG || cnv.cnv8[1]);
	buf[1] = cnv.cnv8[0];

	cnv.cnv16[0] = seq_cnt;
	buf[2] = (  << SEQ_FLG || cnv.cnv8[1]);
	buf[3] = cnv.cnv8[0];

	/* TYPE = 0 TM, TYPE = 1 TC*/
	if(type == TM) {
		buf[6] = ECSS_PUS_VER << PUS_VER ;	
	} else if(type == TC) {
		buf[6] = ( ECSS_SEC_HDR_FIELD_FLG << SEC_HDR_FIELD_FLG || ECSS_PUS_VER << PUS_VER || ack);	
	} else {
		/*return error*/
	}


	buf[7] = ser_type;
	buf[8] = ser_subtype;
	buf[9] = id; /*source or destination*/ 

	if(ser_type == TC_VERIFICATION_SERVICE) {
		cnv.cnv16[0] = tc_pkt_id;
		cnv.cnv16[1] = tc_pkt_seq_ctrl;

		buf[10] = cnv.cnv8[1];
		buf[11] = cnv.cnv8[0];
		buf[12] = cnv.cnv8[3];
		buf[13] = cnv.cnv8[2];

		if(ser_subtype == ACCEPTANCE_REPORT || ser_subtype == EXECUTION_REPORT || ser_subtype == COMPLETION_REPORT ) {
			buf_pointer = 14;
		} else if(ser_subtype == ACCEPTANCE_REPORT_FAIL || ser_subtype == EXECUTION_REPORT_FAIL|| ser_subtype == COMPLETION_REPORT_FAIL) {
			buf[14] = code; 
			buf_pointer = 15;
		} else {
			/*return error*/
		}
	} else if(ser_type == HOUSEKEEPING_SERVICE ) {

		buf[10] = sid;
		if(ser_subtype == 21 ) {
			buf_pointer = 11;
		} else if(ser_subtype == 23) {

			if( sid == 3) {
				cnv.cnv32 = time.now();
				buf[12] = cnv.cnv8[3];
				buf[13] = cnv.cnv8[2];
				buf[14] = cnv.cnv8[1];
				buf[15] = cnv.cnv8[0];
			}
			buf_pointer = 16;
		} else {
			/*return error*/
		}
		

	}else if(ser_type == FUNCTION_MANAGEMENT_SERVICE &&  ser_subtype == 1) {

		buf[10] = fid;
		cnv.cnv32 = value;
		buf[11] = cnv.cnv8[3];
		buf[12] = cnv.cnv8[2];
		buf[13] = cnv.cnv8[1];
		buf[14] = cnv.cnv8[0];

		buf_pointer = 15;

	} else {
		/*return error*/
	}

	/*check if this is correct*/
	cnv.cnv16[0] = buf_pointer -6;
	buf[4] = cnv.cnv8[1];
	buf[5] = cnv.cnv8[0];

	cnv.cnv16[0] = crc(buf, cnt);
	buf[buf_pointer++] = cnv.cnv8[1];
	buf[buf_pointer++] = cnv.cnv8[0];

}


void routing( uint8_t * pkt) {
	
}
