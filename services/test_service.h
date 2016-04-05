#ifndef TEST_SERVICE_H
#define TEST_SERVICE_H

#include <stdint.h>
#include "services.h"

extern tc_tm_pkt * get_pkt();
extern uint32_t time_now();
extern SAT_returnState crt_pkt(tc_tm_pkt *pkt, uint16_t app_id, uint8_t type, uint8_t ack, uint8_t ser_type, uint8_t ser_subtype, uint16_t dest_id);
extern SAT_returnState route_pkt(tc_tm_pkt *pkt);

SAT_returnState test_app(tc_tm_pkt *pkt);

SAT_returnState test_crt_pkt(tc_tm_pkt **pkt, TC_TM_app_id dest_id);

#endif