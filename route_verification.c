#include "route_verification.h"
#include "housekeeping_service.h"

uint8_t route_pkt(struct tc_tm_pkt *pkt) {
	uint16_t id;

	if(pkt->type == TC) {
		id = pkt->app_id;
	} else if(pkt->type == TM) {
		id = pkt->dest_id;
	} else {
		return R_ERROR;
	}

	if(id == OBC) {
		if(pkt->ser_type == TC_HOUSEKEEPING_SERVICE && ( pkt->ser_subtype == 21 || pkt->ser_subtype == 23)) {
			hk_app(&pkt);
			return R_OK;
		} else if(pkt->ser_type == TC_FUNCTION_MANAGEMENT_SERVICE && pkt->ser_subtype == 1) {
			fid = pkt->data[0];
			if(fid == 1 || fid == 2 || fid == 3 ){
				power_control_app(&pkt);
				return R_OK;
			} else {
				return R_ERROR;
			}
		} else {
			return R_ERROR;
		}
	} else if(id == EPS) {
		if(pkt->ser_type == TC_HOUSEKEEPING_SERVICE && pkt->ser_subtype == 21) {
			hk_app(&pkt);
			return R_OK;
		} else {
			return R_ERROR;
		}
	} else if(id == ADCS) {

	} else if(id == COMMS) {
		if(pkt->ser_type == TC_HOUSEKEEPING_SERVICE && pkt->ser_subtype == 21) {
			hk_app(&pkt);
			return R_OK;
		} else {
			return R_ERROR;
		}
	} else if(id == IAC) {

	} else if(id == GND) {
		if(pkt->ser_type == TC_HOUSEKEEPING_SERVICE && pkt->ser_subtype == 25) {
			hk_app(&pkt);
			return R_OK;
		} else {
			return R_ERROR;
		}

	} else {
		return R_ERROR;
	}

        return R_ERROR;
}