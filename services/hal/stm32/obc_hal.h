#ifndef __OBC_HAL_H
#define __OBC_HAL_H

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include <cmsis_os.h>
#include "../../services.h"

//ToDo
//  improve return types
//  need to make the change from the two different typedefs

extern UART_HandleTypeDef huart2;
extern RTC_HandleTypeDef hrtc;

void HAL_obc_SD_ON();

void HAL_obc_SD_OFF();

void HAL_obc_delay(uint32_t sec);

void HAL_eps_uart_tx(uint8_t *buf, uint16_t size);

SAT_returnState HAL_eps_uart_rx(uint8_t *c);

void HAL_su_uart_tx(uint8_t *buf, uint16_t size);

SAT_returnState HAL_su_uart_rx(uint8_t *c);

void HAL_reset_source(uint8_t *src);

void HAL_obc_setTime(uint8_t hours, uint8_t mins, uint8_t sec);

void HAL_obc_getTime(uint8_t *hours, uint8_t *mins, uint8_t *sec);

void HAL_obc_enableBkUpAccess();

#endif