#ifndef __HOUSEKEEPING_SERVICE_H
#define __HOUSEKEEPING_SERVICE_H

#include <stdint.h>

#include "tc_tm.h"
#include "route_verification.h"

struct _obc_status {
	uint8_t mode;
	uint8_t batt_curr;
	uint8_t batt_volt;
	uint8_t bus_3v3_curr;
	uint8_t bus_5v_curr;
	uint8_t temp_eps;
	uint8_t temp_batt;
	uint8_t temp_comms;

}obc_status;

uint8_t hk_app(struct tc_tm_pkt *pkt);

uint8_t hk_crt_pkt_TC(struct tc_tm_pkt *pkt, uint16_t app_id, uint8_t sid);

uint8_t hk_crt_pkt_TM(struct tc_tm_pkt *pkt, uint16_t app_id, uint8_t sid);

void hk_SCH();

void clear_wod();

#endif