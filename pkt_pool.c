#include "pkt_pool.h"

void *get_pkt() {
	for(uint8_t i = 0; i < MAX_POOL_PKT; i++) {
		if(pkt_pool.free[i] == 0) {
			return &pkt_pool.pkt[i];
		}
	}

	return NULL;
}

OBC_returnStateTypedef free_pkt(struct tc_tm_pkt *pkt) {
	for(uint8_t i = 0; i < MAX_POOL_PKT; i++) {
		if(&pkt_pool.pkt[i] == pkt) {
			pkt_pool.free[i] = 0;
			return R_OK;
		}
	}

	return R_ERROR;
}