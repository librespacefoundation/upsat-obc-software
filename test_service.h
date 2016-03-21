#ifndef TEST_SERVICE_H
#define TEST_SERVICE_H

#include <stdint.h>

#include "tc_tm.h"

extern void *get_pkt(uint8_t mode);

OBC_returnStateTypedef test_app(tc_tm_pkt *pkt);

OBC_returnStateTypedef test_crt_pkt(tc_tm_pkt *pkt);

#endif