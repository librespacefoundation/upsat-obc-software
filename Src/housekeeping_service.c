#include "../Inc/housekeeping_service.h"

void hk_SCH() {

  struct tc_tm_pkt pkt;
  
	hk_crt_pkt_TC(&pkt, EPS, 1);
	route_pkt(&pkt);
	hk_crt_pkt_TC(&pkt, COMMS, 1);
	route_pkt(&pkt);
	//delay(59) sec;
	hk_crt_pkt_TM(&pkt, GND, 4);
	route_pkt(&pkt);
	clear_wod();
}

void clear_wod() {
		obc_status.batt_curr = 0;
		obc_status.batt_volt = 0;
		obc_status.bus_3v3_curr = 0;
		obc_status.bus_5v_curr = 0;
		obc_status.temp_eps = 0;
		obc_status.temp_batt = 0;
		obc_status.temp_comms = 0;
}

OBC_returnStateTypedef hk_app(tc_tm_pkt *pkt) {
	uint8_t res, did, fid;

	if(pkt->ser_type == TC_HOUSEKEEPING_SERVICE &&  pkt->ser_subtype == 21) {
		hk_crt_pkt_TM(pkt, pkt->dest_id, pkt->data[0]);
		route_pkt(pkt);
	} else if(pkt->ser_type == TC_HOUSEKEEPING_SERVICE &&  pkt->ser_subtype == 23) {
		if(pkt->app_id == EPS) {
			obc_status.batt_curr = pkt->data[1];
			obc_status.batt_volt = pkt->data[2];
			obc_status.bus_3v3_curr = pkt->data[3];
			obc_status.bus_5v_curr = pkt->data[4];
			obc_status.temp_eps = pkt->data[5];
			obc_status.temp_batt = pkt->data[6];
		} else if(pkt->app_id == COMMS) {
			obc_status.temp_comms = pkt->data[1];
		}
	}

	did = pkt->data[0];
	fid = pkt->data[4];
	res = power_control_app_api( did, fid);
	return R_OK;
}

OBC_returnStateTypedef hk_crt_pkt_TC(tc_tm_pkt *pkt, uint16_t app_id, uint8_t sid) {

	pkt->type = TC;
	pkt->app_id = app_id; 
	pkt->dest_id = OBC;

	pkt->data[0] = sid;

	pkt->ser_type = TC_HOUSEKEEPING_SERVICE;

	pkt->ser_subtype = 21;

	return R_OK;
}

OBC_returnStateTypedef hk_crt_pkt_TM(tc_tm_pkt *pkt, uint16_t app_id, uint8_t sid) {
	union _cnv cnv;

	pkt->type = TC;
	pkt->app_id = OBC; 
	pkt->dest_id = app_id;

	pkt->data[0] = sid;

	pkt->ser_type = TC_HOUSEKEEPING_SERVICE;

	if(sid == 3) {
		pkt->ser_subtype = 21;

		//cnv.cnv32 = time.now();
		pkt->data[1] = cnv.cnv8[3];
		pkt->data[2] = cnv.cnv8[2];
		pkt->data[3] = cnv.cnv8[1];
		pkt->data[4] = cnv.cnv8[0];
	} else if(sid == 4) {
		pkt->ser_subtype = 25;

		pkt->data[1] = obc_status.mode;
		pkt->data[2] = obc_status.batt_curr;
		pkt->data[3] = obc_status.batt_volt;
		pkt->data[4] = obc_status.bus_3v3_curr;
		pkt->data[5] = obc_status.bus_5v_curr;
		pkt->data[6] = obc_status.temp_eps;
		pkt->data[7] = obc_status.temp_batt;
		pkt->data[8] = obc_status.temp_comms;
	}
	return R_OK;
}