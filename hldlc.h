#ifndef __HLDLC_H
#define __HLDLC_H

#include <stdint.h>
#include "tc_tm.h"

#define HLDLC_START_FLAG        0x7E
#define HLDLC_CONTROL_FLAG      0x7D

OBC_returnStateTypedef HLDLC_deframe(uint8_t *buf, uint16_t *cnt, uint8_t c, uint16_t *size);

OBC_returnStateTypedef HLDLC_frame(uint8_t *c, uint8_t *buf, uint16_t * cnt, const uint16_t size);

#endif
