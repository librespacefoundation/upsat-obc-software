#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "tc_tm.h"

#define TEST_ARRAY 100

extern uint8_t HLDLC_deframe(uint8_t *buf, uint16_t *cnt, uint8_t c);
extern uint8_t HLDLC_frame( uint8_t *c, uint8_t *buf, uint16_t * cnt, const uint16_t size);

uint8_t serial_app(UART_HandleTypeDef huart, uint8_t tx_flag);

#endif