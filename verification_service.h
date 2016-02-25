#ifndef VERIFICATION_SERVICE_H
#define VERIFICATION_SERVICE_H

#include <stdint.h>

#include "tc_tm.h"

uint8_t verify_pkt( struct tc_tm_pkt *pkt, uint8_t step, uint8_t res);

uint8_t verify_crt_pkt( struct tc_tm_pkt *pkt, struct tc_tm_pkt *out, uint8_t res);

#endif