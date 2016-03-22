#ifndef __VERIFICATION_SERVICE_H
#define __VERIFICATION_SERVICE_H

#include <stdint.h>
#include "tc_tm.h"

//ToDo
//  check verify_pkt

extern void *get_pkt(uint8_t mode);
extern uint32_t time_now();
extern OBC_returnStateTypedef crt_pkt(tc_tm_pkt *pkt, uint16_t app_id, uint8_t type, uint8_t ack, uint8_t ser_type, uint8_t ser_subtype, uint16_t dest_id);
extern OBC_returnStateTypedef route_pkt(tc_tm_pkt *pkt);

OBC_returnStateTypedef verification_app(tc_tm_pkt *pkt, OBC_returnStateTypedef res); 

OBC_returnStateTypedef verification_crt_pkt(tc_tm_pkt *pkt, tc_tm_pkt *out, OBC_returnStateTypedef res);

#endif