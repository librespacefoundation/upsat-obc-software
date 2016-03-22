#ifndef __VERIFICATION_SERVICE_H
#define __VERIFICATION_SERVICE_H

#include <stdint.h>

#include "../Inc/services.h"

SAT_returnState verify_pkt(tc_tm_pkt *pkt, uint8_t step, uint8_t res);

SAT_returnState verify_crt_pkt(tc_tm_pkt *pkt, tc_tm_pkt *out, uint8_t res, uint8_t subtype);

#endif