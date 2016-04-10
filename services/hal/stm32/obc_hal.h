#ifndef __OBC_HAL_H
#define __OBC_HAL_H

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include <cmsis_os.h>
#include "../../services.h"

//ToDo
//  improve return types
//  need to make the change from the two different typedefs

extern struct _obc_data obc_data;
extern UART_HandleTypeDef huart2;
extern RTC_HandleTypeDef hrtc;

void HAL_obc_SD_ON();

void HAL_obc_SD_OFF();

void HAL_obc_delay(uint32_t sec);

void HAL_OBC_UART_IRQHandler(UART_HandleTypeDef *huart);

void UART_OBC_Receive_IT(UART_HandleTypeDef *huart);

void HAL_eps_uart_tx(uint8_t *buf, uint16_t size);

SAT_returnState HAL_eps_uart_rx();

void HAL_su_uart_tx(uint8_t *buf, uint16_t size);

SAT_returnState HAL_su_uart_rx(uint8_t *c);

void HAL_reset_source(uint8_t *src);

void HAL_obc_setTime(uint8_t hours, uint8_t mins, uint8_t sec);

void HAL_obc_getTime(uint8_t *hours, uint8_t *mins, uint8_t *sec);

void HAL_obc_setDate(uint8_t mon, uint8_t date, uint8_t year);

void HAL_obc_getDate(uint8_t *mon, uint8_t *date, uint8_t *year);

void HAL_obc_enableBkUpAccess();

uint32_t * HAL_obc_BKPSRAM_BASE();

uint32_t HAL_obc_GetTick();

#endif