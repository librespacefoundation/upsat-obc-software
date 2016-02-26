#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "tc_tm.h"

#define TEST_ARRAY 100

extern OBC_returnStateTypedef HLDLC_deframe(uint8_t *buf, uint16_t *cnt, uint8_t c);
extern OBC_returnStateTypedef HLDLC_frame( uint8_t *c, uint8_t *buf, uint16_t * cnt, const uint16_t size);

OBC_returnStateTypedef serial_app(UART_HandleTypeDef huart, uint8_t tx_flag);

#endif