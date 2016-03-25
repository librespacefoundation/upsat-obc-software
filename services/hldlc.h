#ifndef __HLDLC_H
#define __HLDLC_H

#include <stdint.h>
#include "services.h"

#define HLDLC_START_FLAG        0x7E
#define HLDLC_CONTROL_FLAG      0x7D

//ToDo

//Finished
//	need to check C_ASSERT(cnt == 0 && c != HLDLC_START_FLAG)

SAT_returnState HLDLC_deframe(uint8_t *buf, uint16_t *cnt, const uint8_t c, uint16_t *size);

SAT_returnState HLDLC_frame(uint8_t *c, uint8_t *buf, uint16_t * cnt, const uint16_t size);

#endif
