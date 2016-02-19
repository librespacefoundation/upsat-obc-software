
#define HDLC_START_FLAG		0x7E
#define HDLC_CONTROL_FLAG	0x7D


/* it should return the status*/
void HDLC_deframe( int8_t *buf, int16_t * cnt, uint8_t c) {

	if( cnt != 0 && c == HDLC_START_FLAG ) {
		* cnt = 0;
		/* return end of packet*/
	} else if( cnt == 0 && c != HDLC_START_FLAG ) {
		/* return error*/
	} else if( cnt != 0 && *buf[*cnt-1] == HDLC_CONTROL_FLAG ) {
		if( c == 0x5E ) { *buf[*cnt-1] = HDLC_START_FLAG; }
		else if( c == 0x5D ) { *buf[*cnt-1] = HDLC_CONTROL_FLAG; }
		else { /* return error*/ }
		/* return ok*/
	} else {
		*buf[*cnt] = c;
		(*cnt)++
		/* return ok*/
	}
}

/* it should return the status*/
void HDLC_frame( uint8_t * c, const uint8_t *buf, uint16_t * cnt, const uint16_t size) {

	assert( *cnt < size);

	if( *cnt == 0 ) {

		*c = HDLC_START_FLAG;
		(*cnt)++;
		/* return ok*/
	} else if( cnt >= size ) {
		/* return end of trasmitions*/
	} else if( cnt != 0 && *buf[*cnt] == HDLC_START_FLAG ) {
		*c = HDLC_CONTROL_FLAG;
		*buf[*cnt] = 0x5E;
		/* return ok*/
	} else if( cnt != 0 &&  *buf[*cnt] == HDLC_CONTROL_FLAG ) {
		*c = HDLC_CONTROL_FLAG;
		*buf[*cnt] = 0x5D;
		/* return ok*/
	} else {
		*c = *buf[*cnt-1] ;
		(*cnt)++;
		/* return ok*/
	}
}