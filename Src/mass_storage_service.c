#include "../Inc/mass_storage_service.h"

SAT_returnState mass_storage_app(tc_tm_pkt *pkt) {

	union _cnv cnv;
	uint8_t sid, mode;

	if(pkt->ser_subtype == 1 || pkt->ser_subtype == 2 ) {
		sid = pkt->data[0];
		mode = pkt->ser_subtype;
		mass_storage_mode_api(sid, mode);
		return SATR_OK;
	} else if(pkt->ser_subtype == 11 ) {
		sid = pkt->data[4];
		// check endianess
		cnv.cnv8[0] = pkt->data[0];
		cnv.cnv8[1] = pkt->data[1];
		cnv.cnv8[2] = pkt->data[2];
		cnv.cnv8[3] = pkt->data[3];
		mode = pkt->ser_subtype;
		mass_storage_delete_api(sid, cnv.cnv32);
		return SATR_OK;
	}
}