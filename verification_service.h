#ifndef __VERIFICATION_SERVICE_H
#define __VERIFICATION_SERVICE_H

#include <stdint.h>

#include "tc_tm.h"

OBC_returnStateTypedef verify_pkt(struct tc_tm_pkt *pkt, uint8_t step, uint8_t res);

OBC_returnStateTypedef verify_crt_pkt(struct tc_tm_pkt *pkt, struct tc_tm_pkt *out, uint8_t res, uint8_t subtype);

#endif