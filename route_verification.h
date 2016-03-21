#ifndef ROUTE_VERIFICATION_H
#define ROUTE_VERIFICATION_H

#include <stdint.h>

#include "tc_tm.h"

//temp
#define TEST_ARRAY 1024

extern void *get_pkt(uint8_t mode);
extern uint32_t time_now();
extern OBC_returnStateTypedef hk_app(tc_tm_pkt *pkt);
extern OBC_returnStateTypedef power_control_app(tc_tm_pkt *pkt);
extern OBC_returnStateTypedef mass_storage_app(tc_tm_pkt *pkt);

OBC_returnStateTypedef route_pkt(tc_tm_pkt *pkt);

OBC_returnStateTypedef import_eps_pkt();

OBC_returnStateTypedef export_eps_pkt(tc_tm_pkt *pkt);

#endif
