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
    //HAL_UART_Transmit_DMA(&huart2, buf, size, 10);
}

SAT_returnState HAL_eps_uart_rx(uint8_t *c) {

    HAL_StatusTypeDef res;

    res = HAL_UART_Receive(&huart2, c, 1, 10);
    if(res == HAL_OK) { return SATR_OK; }
    else if(res == HAL_TIMEOUT) { return SATR_ERROR; }
    
    return SATR_ERROR;
}

void HAL_su_uart_tx(uint8_t *buf, uint16_t size) {
    HAL_UART_Transmit(&huart2, buf, size, 10);
    //HAL_UART_Transmit_DMA(&huart2, buf, size, 10);
}

SAT_returnState HAL_su_uart_rx(uint8_t *c) {

    HAL_StatusTypeDef res;

    res = HAL_UART_Receive(&huart2, c, 1, 10);
    if(res == HAL_OK) { return SATR_OK; }
    else if(res == HAL_TIMEOUT) { return SATR_ERROR; }
    
    return SATR_ERROR;
}

void HAL_reset_source(uint8_t *src) {

    uint8_t temp;

    temp = __HAL_RCC_GET_FLAG(RCC_FLAG_BORRST);
    temp |= (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) << 1);
    temp |= (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) << 2);
    temp |= (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST) << 3);
    temp |= (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) << 4);
    temp |= (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST) << 5);
    temp |= (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST) << 6);
 
    __HAL_RCC_CLEAR_RESET_FLAGS();

}