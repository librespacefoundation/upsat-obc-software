#ifndef __PKT_POOL_H
#define __PKT_POOL_H

#include <stdint.h>
#include "services.h"

#define PKT_TIMEOUT 1000 /*in seconds*/

#define POOL_PKT_SIZE    20

struct _pkt_pool{
    tc_tm_pkt pkt[POOL_PKT_SIZE];
    uint8_t free[POOL_PKT_SIZE];
    uint32_t time[POOL_PKT_SIZE];
    uint8_t data[POOL_PKT_SIZE][MAX_PKT_DATA];
};

//ToDo
//	add assertions
//	finish definitions

extern uint32_t time_now();

extern struct _pkt_pool pkt_pool;

tc_tm_pkt * get_pkt();

SAT_returnState free_pkt(tc_tm_pkt *pkt);

SAT_returnState pkt_pool_INIT();

void pkt_pool_GC();

#endif