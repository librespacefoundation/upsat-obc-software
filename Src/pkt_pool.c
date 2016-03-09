#include "../Inc/pkt_pool.h"

void *get_pkt(uint8_t mode) {
	
	uint8_t start;

	if(mode == NORMAL) {
		start = 0;
	} else if(mode == EXTENDED) {
		start = EXT_POOL_PKT_START;
	} else {
		return NULL;
	}

	for(uint8_t i = start; i < MAX_POOL_PKT; i++) {
		if(pkt_pool.free[i] == 0) {
			pkt_pool.free[i] = 1;
			//pkt_pool.time[i] = time.now();
			return &pkt_pool.pkt[i];
		}
	}

	return NULL;
}

OBC_returnStateTypedef free_pkt(tc_tm_pkt *pkt) {
	for(uint8_t i = 0; i < MAX_POOL_PKT; i++) {
		if(&pkt_pool.pkt[i] == pkt) {
			pkt_pool.free[i] = 0;
			return R_OK;
		}
	}

	return R_ERROR;
}

OBC_returnStateTypedef pkt_pool_INIT() {

	for(uint8_t i = 0; i < MAX_POOL_NRM_PKT; i++) {
		pkt_pool.pkt[i].data = data_NRM[i];
	}

	for(uint8_t i = 0; i < MAX_POOL_EXT_PKT; i++) {
		pkt_pool.pkt[i+EXT_POOL_PKT_START].data = data_EXT[i];
	}
	return R_OK;
}

void pkt_pool_GC() {

	for(uint8_t i = start; i < MAX_POOL_PKT; i++) {
		if(pkt_pool.free[i] == 1 && pkt_pool.time[i] - time.now() > PKT_TIMEOUT) {
			//pkt_pool.free[i] = 0;
			// error
		}
	}

	return R_OK;
}
