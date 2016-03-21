#ifndef __VERIFICATION_SERVICE_H
#define __VERIFICATION_SERVICE_H

#include <stdint.h>

#include "tc_tm.h"

//ToDo
//  check verify_pkt

extern void *get_pkt(uint8_t mode);

OBC_returnStateTypedef verification_app(tc_tm_pkt *pkt, OBC_returnStateTypedef res); 

OBC_returnStateTypedef verification_crt_pkt(tc_tm_pkt *pkt, tc_tm_pkt *out, OBC_returnStateTypedef res);

#endif