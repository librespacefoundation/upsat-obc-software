#include "obc_hal.h"


#undef __FILE_ID__
#define __FILE_ID__ 13

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

    *src = __HAL_RCC_GET_FLAG(RCC_FLAG_BORRST);
    *src |= (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) << 1);
    *src |= (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) << 2);
    *src |= (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST) << 3);
    *src |= (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) << 4);
    *src |= (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST) << 5);
    *src |= (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST) << 6);
 
    __HAL_RCC_CLEAR_RESET_FLAGS();

}

void HAL_obc_setTime(uint8_t hours, uint8_t mins, uint8_t sec) {

  RTC_TimeTypeDef sTime;

  sTime.Hours = hours;
  sTime.Minutes = mins;
  sTime.Seconds = sec;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);


}

void HAL_obc_getTime(uint8_t *hours, uint8_t *mins, uint8_t *sec) {

  RTC_TimeTypeDef sTime;

  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

   *hours = sTime.Hours;
   *mins = sTime.Minutes;
   *sec = sTime.Seconds;  
}

//void HAL_obc_setDate();
//
//  RTC_DateTypeDef sDate;
//
//  sDate.WeekDay = RTC_WEEKDAY_FRIDAY;
//  sDate.Month = RTC_MONTH_APRIL;
//  sDate.Date = 1;
//  sDate.Year = 16;
//
//  HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
//
//}
//
//void HAL_obc_getDate();
//
//}

void HAL_obc_enableBkUpAccess() {
  
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_BKPSRAM_CLK_ENABLE();
  
}

uint32_t * HAL_obc_BKPSRAM_BASE() {
  return (uint32_t *)BKPSRAM_BASE;
}
