#ifndef __VERIFICATION_SERVICE_H
#define __VERIFICATION_SERVICE_H

#include <stdint.h>

#include "tc_tm.h"

//ToDo
//  check verify_pkt

OBC_returnStateTypedef verification_app(tc_tm_pkt *pkt, uint8_t res);

OBC_returnStateTypedef verification_crt_pkt(tc_tm_pkt *pkt, tc_tm_pkt *out, uint8_t res);

#endif