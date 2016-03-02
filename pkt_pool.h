#ifndef __PKT_POOL_H
#define __PKT_POOL_H

#include <stdint.h>
#include "tc_tm.h"

#define MAX_POOL_PKT 10

struct _pkt_pool{
	struct tc_tm_pkt pkt[MAX_POOL_PKT];
	uint8_t	free[MAX_POOL_PKT];
}pkt_pool;

void *get_pkt();

OBC_returnStateTypedef free_pkt(struct tc_tm_pkt *pkt);

#endif