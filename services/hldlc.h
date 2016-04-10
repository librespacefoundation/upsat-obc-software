#ifndef __HLDLC_H
#define __HLDLC_H

#include <stdint.h>
#include "services.h"

//ToDo

//Finished
//	need to check C_ASSERT(cnt == 0 && c != HLDLC_START_FLAG)

SAT_returnState HLDLC_deframe(uint8_t *buf_in, uint8_t *buf_out, uint16_t *size);

SAT_returnState HLDLC_frame(uint8_t *buf_in, uint8_t *buf_out, uint16_t *size);

#endif
