#include "obc_hal.h"

void HAL_obc_delay(uint32_t sec) {
	osDelay(sec);
}

void HAL_obc_SD_ON() {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
}

void HAL_obc_SD_OFF() {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
}

void HAL_eps_uart_tx(uint8_t *buf, uint16_t size) {
    HAL_UART_Transmit(&huart2, buf, size, 10);
}

SAT_returnState HAL_eps_uart_rx(uint8_t *c) {

    HAL_StatusTypeDef res;

    res = HAL_UART_Receive(&huart2, c, 1, 10);
    if(res == HAL_OK) { return SATR_OK; }
    else if(res == HAL_TIMEOUT) { return SATR_ERROR; }
    
    return SATR_ERROR;
}