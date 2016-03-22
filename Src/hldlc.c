#include "../Inc/hldlc.h"

/* it should return the status*/
SAT_returnState HLDLC_deframe(uint8_t *buf, uint16_t *cnt, uint8_t c, uint16_t *size) {

	if( cnt != 0 && c == HLDLC_START_FLAG ) {
		*cnt = 0;
		return SATR_EOT;
	} else if( cnt == 0 && c != HLDLC_START_FLAG ) {
		*size = *cnt;
		*cnt = 0;
		return SATR_ERROR;
	} else if( cnt != 0 && buf[(*cnt)-1] == HLDLC_CONTROL_FLAG ) {
		if( c == 0x5E ) { buf[*cnt-1] = HLDLC_START_FLAG; }
		else if( c == 0x5D ) { buf[*cnt-1] = HLDLC_CONTROL_FLAG; }
		else { return SATR_ERROR; }
		return SATR_OK;
	} else {
		buf[*cnt] = c;
		(*cnt)++;
		return SATR_OK;
	}
}

/* it should return the status*/
SAT_returnState HLDLC_frame(uint8_t *c, uint8_t *buf, uint16_t *cnt, const uint16_t size) {

	//assert( *cnt < size);

	if( *cnt == 0 ) {

		*c = HLDLC_START_FLAG;
		(*cnt)++;
		return SATR_OK;
	} else if( *cnt > size) {
        *c = HLDLC_START_FLAG;
		return SATR_EOT;
	} else if( cnt != 0 && buf[(*cnt)-1] == HLDLC_START_FLAG ) {
		*c = HLDLC_CONTROL_FLAG;
		buf[(*cnt)-1] = 0x5E;
		return SATR_OK;
	} else if( cnt != 0 &&  buf[(*cnt)-1] == HLDLC_CONTROL_FLAG ) {
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