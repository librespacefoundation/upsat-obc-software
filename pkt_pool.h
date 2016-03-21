#ifndef __PKT_POOL_H
#define __PKT_POOL_H

#include <stdint.h>
#include "tc_tm.h"

#define PKT_TIMEOUT 1000 /*in seconds*/

#define MAX_POOL_NRM_PKT    8
#define MAX_POOL_EXT_PKT    2
#define MAX_POOL_PKT        MAX_POOL_EXT_PKT+MAX_POOL_NRM_PKT
#define EXT_POOL_PKT_START  MAX_POOL_NRM_PKT

struct _pkt_pool{
    tc_tm_pkt pkt[MAX_POOL_PKT];
    uint8_t free[MAX_POOL_PKT];
    uint32_t time[MAX_POOL_PKT];

    uint8_t data_NRM[MAX_POOL_NRM_PKT][MAX_PKT_DATA];
    uint8_t data_EXT[MAX_POOL_EXT_PKT][MAX_EX_PKT_DATA];
}pkt_pool;

void *get_pkt(uint8_t mode);

OBC_returnStateTypedef free_pkt(tc_tm_pkt *pkt);

OBC_returnStateTypedef pkt_pool_INIT();

void pkt_pool_GC();

#endif