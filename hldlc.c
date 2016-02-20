#include "hldlc.h"


/* it should return the status*/
void HLDLC_deframe( uint8_t *buf, uint16_t * cnt, uint8_t c) {

	if( cnt != 0 && c == HDLC_START_FLAG ) {
		* cnt = 0;
		/* return end of packet*/
	} else if( cnt == 0 && c != HDLC_START_FLAG ) {
		/* return error*/
	} else if( cnt != 0 && buf[(*cnt)-1] == HDLC_CONTROL_FLAG ) {
		if( c == 0x5E ) { buf[*cnt-1] = HDLC_START_FLAG; }
		else if( c == 0x5D ) { buf[*cnt-1] = HDLC_CONTROL_FLAG; }
		else { /* return error*/ }
		/* return ok*/
	} else {
		buf[*cnt] = c;
        (*cnt)++;
		/* return ok*/
	}
}

/* it should return the status*/
uint8_t HLDLC_frame( uint8_t *c, uint8_t *buf, uint16_t * cnt, const uint16_t size) {

	//assert( *cnt < size);

	if( *cnt == 0 ) {

		*c = HDLC_START_FLAG;
		(*cnt)++;
		return R_OK;
	} else if( *cnt > size) {
        *c = HDLC_START_FLAG;
		return R_EOT;
	} else if( cnt != 0 && buf[*cnt] == HDLC_START_FLAG ) {
		*c = HDLC_CONTROL_FLAG;
		buf[*cnt] = 0x5E;
		return R_OK;
	} else if( cnt != 0 &&  buf[*cnt] == HDLC_CONTROL_FLAG ) {
		*c = HDLC_CONTROL_FLAG;
		buf[*cnt] = 0x5D;
		return R_OK;
	} else {
		*c = buf[(*cnt)-1] ;
		(*cnt)++;
		return R_OK;
	}
    return R_ERROR;
}