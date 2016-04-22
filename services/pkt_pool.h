#ifndef __PKT_POOL_H
#define __PKT_POOL_H

#include <stdint.h>
#include "services.h"
#include "system.h"

#define PKT_TIMEOUT 1000 /*in seconds*/

#ifndef POOL_PKT_SIZE
#define POOL_PKT_SIZE   30
#endif

struct _pkt_pool{
    tc_tm_pkt pkt[POOL_PKT_SIZE];
    uint8_t free[POOL_PKT_SIZE];
    uint32_t time[POOL_PKT_SIZE];
    uint8_t data[POOL_PKT_SIZE][MAX_PKT_DATA];
};

//ToDo
//	add assertions
//	finish definitions

tc_tm_pkt * get_pkt();

SAT_returnState free_pkt(tc_tm_pkt *pkt);

SAT_returnState pkt_pool_INIT();

void pkt_pool_GC();

#endif