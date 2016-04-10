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
    HAL_StatusTypeDef res;
    //HAL_UART_Transmit(&huart2, buf, size, 10);
    for(;;) { // should use hard limits
        res = HAL_UART_Transmit_DMA(&huart2, buf, size);
        if(res == HAL_OK) { break; }
        osDelay(10);
    }
}

SAT_returnState HAL_eps_uart_rx() {

    if(huart2.RxState == HAL_UART_STATE_READY) {
        obc_data.eps_uart_size = huart2.RxXferSize - huart2.RxXferCount;
        HAL_UART_Receive_IT(&huart2, obc_data.eps_uart_buf, OBC_UART_BUF_SIZE);
        return SATR_EOT;
    }
    return SATR_OK;
}

/**
  * @brief  This function handles UART interrupt request.
  * @param  huart: pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_OBC_UART_IRQHandler(UART_HandleTypeDef *huart)
{
  uint32_t tmp1 = 0U, tmp2 = 0U;

  tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE);
  tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_RXNE);
  /* UART in mode Receiver ---------------------------------------------------*/
  if((tmp1 != RESET) && (tmp2 != RESET))
  { 
    UART_OBC_Receive_IT(huart);
  }
}

/**
  * @brief  Receives an amount of data in non blocking mode 
  * @param  huart: pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
void UART_OBC_Receive_IT(UART_HandleTypeDef *huart)
{
    uint8_t c;

    c = (uint8_t)(huart->Instance->DR & (uint8_t)0x00FFU);
    if(huart->RxXferSize == huart->RxXferCount && c == HLDLC_START_FLAG) {
      *huart->pRxBuffPtr++ = c;
      huart->RxXferCount--;
      //start timeout
    } else if(c == HLDLC_START_FLAG && (huart->RxXferSize - huart->RxXferCount) < TC_MIN_PKT_SIZE) {
      //error
      //event log
      //reset buffers & pointers
      //start timeout
    } else if(c == HLDLC_START_FLAG) {
      *huart->pRxBuffPtr++ = c;
      huart->RxXferCount--;
      
      __HAL_UART_DISABLE_IT(huart, UART_IT_RXNE);

      /* Disable the UART Parity Error Interrupt */
      __HAL_UART_DISABLE_IT(huart, UART_IT_PE);

      /* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
      __HAL_UART_DISABLE_IT(huart, UART_IT_ERR);

	  /* Rx process is completed, restore huart->RxState to Ready */
      huart->RxState = HAL_UART_STATE_READY;
    } else {
      *huart->pRxBuffPtr++ = c;
      huart->RxXferCount--;
    }

    if(huart->RxXferCount == 0U) // errror
    {


    }

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

void HAL_obc_setDate(uint8_t mon, uint8_t date, uint8_t year) {

  RTC_DateTypeDef sDate;

//  sDate.WeekDay = RTC_WEEKDAY_FRIDAY;
  sDate.Month = mon;
  sDate.Date = date;
  sDate.Year = year;

  HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

}

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

uint32_t HAL_obc_GetTick() {
  return HAL_GetTick();
}