#ifndef __OBC_HAL_H
#define __OBC_HAL_H

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include <cmsis_os.h>
#include "../../../../services/services.h"

//ToDo
//  improve return types
//  need to make the change from the two different typedefs

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart6;
extern RTC_HandleTypeDef hrtc;

void HAL_obc_SD_ON();

void HAL_obc_SD_OFF();

void HAL_sys_delay(uint32_t millisecs);

void HAL_OBC_UART_IRQHandler(UART_HandleTypeDef *huart);

void UART_OBC_Receive_IT(UART_HandleTypeDef *huart);

void HAL_uart_tx(TC_TM_app_id app_id, uint8_t *buf, uint16_t size);

SAT_returnState HAL_uart_rx(TC_TM_app_id app_id, struct uart_data *data);

void HAL_su_uart_tx(uint8_t *buf, uint16_t size);

SAT_returnState HAL_su_uart_rx(uint8_t *c);

void HAL_reset_source(uint8_t *src);

void HAL_sys_setTime(uint8_t hours, uint8_t mins, uint8_t sec);

void HAL_sys_getTime(uint8_t *hours, uint8_t *mins, uint8_t *sec);

void HAL_sys_setDate(uint8_t mon, uint8_t date, uint8_t year);

void HAL_sys_getDate(uint8_t *mon, uint8_t *date, uint8_t *year);

void HAL_obc_enableBkUpAccess();

uint32_t * HAL_obc_BKPSRAM_BASE();

uint32_t HAL_sys_GetTick();

#endif