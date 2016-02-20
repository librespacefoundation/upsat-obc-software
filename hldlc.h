#define HLDLC_START_FLAG	0x7E
#define HLDLC_CONTROL_FLAG  0x7D

#define R_OK	1
#define R_ERROR	2
#define R_EOT	3

#include <stdio.h>

uint8_t HLDLC_deframe( uint8_t *buf, uint16_t *cnt, uint8_t c);

uint8_t HLDLC_frame( uint8_t *c, uint8_t *buf, uint16_t * cnt, const uint16_t size);