uint8_t verify_pkt( uint8_t * pkt, uint8_t step, uint8_t res) {
	if(type == TC ) {
		if(ack == TC_ACK_NO) {
			return R_NO_VER;
		} else if(ack == TC_ACK_EXE_PROG) {

		} else if(ack == TC_ACK_EXE_COMP) {
			
		} else if(ack == TC_ACK_ALL) {
			
		}
	} else {
		return R_ERROR;
	}
}

uint8_t verify_create_pkt( uint8_t * pkt, uint8_t * out_pkt, uint8_t res, uint8_t error) {

	out.type = TM;
	out.app_id = app_id; 
	out.destination_id = destination_id ;

	out.packetid = packetid;
	out.seq = seq;

	if(res == SUCCESS) {

	} else if(res == FAILURE) {
		out.code = error;
	} else {
		return R_ERROR;
	}

	return R_OK;
}