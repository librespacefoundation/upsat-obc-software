#ifndef __EPS_HAL_H
#define __EPS_HAL_H

#include <stdint.h>
#include "stm32l1xx_hal.h"
#include "../../../../services/services.h"

//ToDo
//  improve return types
//  need to make the change from the two different typedefs

extern struct _eps_data eps_data;
extern UART_HandleTypeDef huart3;
extern RTC_HandleTypeDef hrtc;

void HAL_eps_OBC_ON();

void HAL_eps_OBC_OFF();

void HAL_eps_ADCS_ON();

void HAL_eps_ADCS_OFF();

void HAL_eps_COMMS_ON();

void HAL_eps_COMMS_OFF();

void HAL_eps_SU_ON();

void HAL_eps_SU_OFF();


void HAL_sys_delay(uint32_t sec);

void HAL_EPS_UART_IRQHandler(UART_HandleTypeDef *huart);

void UART_EPS_Receive_IT(UART_HandleTypeDef *huart);

void HAL_uart_tx(TC_TM_app_id app_id, uint8_t *buf, uint16_t size);

SAT_returnState HAL_uart_rx(TC_TM_app_id app_id, struct uart_data *data);

void HAL_reset_source(uint8_t *src);

void HAL_sys_setTime(uint8_t hours, uint8_t mins, uint8_t sec);

void HAL_sys_getTime(uint8_t *hours, uint8_t *mins, uint8_t *sec);

void HAL_sys_setDate(uint8_t mon, uint8_t date, uint8_t year);

void HAL_sys_getDate(uint8_t *mon, uint8_t *date, uint8_t *year);

uint32_t HAL_sys_GetTick();

#endif