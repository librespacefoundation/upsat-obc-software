#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "../Inc/services.h"

#define TEST_ARRAY 100

extern SAT_returnState HLDLC_deframe(uint8_t *buf, uint16_t *cnt, uint8_t c, uint16_t *size);
extern SAT_returnState HLDLC_frame( uint8_t *c, uint8_t *buf, uint16_t * cnt, const uint16_t size);

extern SAT_returnState route_pkt(struct tc_tm_pkt *pkt);
extern SAT_returnState verify_pkt(struct tc_tm_pkt *pkt, uint8_t step, uint8_t res);

SAT_returnState serial_app(UART_HandleTypeDef huart, uint8_t tx_flag);

#endif