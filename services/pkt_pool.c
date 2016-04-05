#include "pkt_pool.h"


#undef __FILE_ID__
#define __FILE_ID__ 10

struct _pkt_pool pkt_pool;

tc_tm_pkt * get_pkt(uint8_t mode) {

    uint8_t start;

    if(!C_ASSERT(mode == NORMAL || mode == EXTENDED) == true) { return NULL; }

    if(mode == NORMAL) {
        start = 0;
    } else if(mode == EXTENDED) {
        start = EXT_POOL_PKT_START;
    }

    for(uint8_t i = start; i < MAX_POOL_PKT; i++) {
        if(pkt_pool.free[i] == true) {
            pkt_pool.free[i] = false;
            pkt_pool.time[i] = time_now();
            pkt_pool.pkt[i].verification_state = SATR_PKT_INIT;
            return &pkt_pool.pkt[i];
        }
    }

    return NULL;
}

SAT_returnState free_pkt(tc_tm_pkt *pkt) {

    if(!C_ASSERT(pkt != NULL) == true) { return SATR_ERROR; }

    for(uint8_t i = 0; i < MAX_POOL_PKT; i++) {
        if(&pkt_pool.pkt[i] == pkt) {
            pkt_pool.free[i] = true;
            return SATR_OK;
        }
    }
    return SATR_ERROR;
}

SAT_returnState pkt_pool_INIT() {

    for(uint8_t i = 0; i < MAX_POOL_NRM_PKT; i++) {
        pkt_pool.pkt[i].data = pkt_pool.data_NRM[i];
    }

    for(uint8_t i = 0; i < MAX_POOL_EXT_PKT; i++) {
        pkt_pool.pkt[i+EXT_POOL_PKT_START].data = pkt_pool.data_EXT[i];
    }
    for(uint8_t i = 0; i < MAX_POOL_PKT; i++) {
       pkt_pool.free[i] = true;
    }
    return SATR_OK;
}

void pkt_pool_GC() {

    for(uint8_t i = 0; i < MAX_POOL_PKT; i++) {
        if(pkt_pool.free[i] == false && pkt_pool.time[i] - time_now() > PKT_TIMEOUT) {
            //pkt_pool.free[i] = 0;
            // error
        }
    }
}
