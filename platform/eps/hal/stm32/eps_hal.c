#include "eps_hal.h"


#undef __FILE_ID__
#define __FILE_ID__ 13

void HAL_sys_delay(uint32_t sec) {
	HAL_Delay(sec);
}

void HAL_eps_OBC_ON() {
    HAL_GPIO_WritePin(GPIOC, GPIO_OBC_SWITCH_Pin, GPIO_PIN_RESET);
}

void HAL_eps_OBC_OFF() {
    HAL_GPIO_WritePin(GPIOC, GPIO_OBC_SWITCH_Pin, GPIO_PIN_SET);
}

void HAL_eps_ADCS_ON() {
    HAL_GPIO_WritePin(GPIOC, GPIO_ADCS_SWITCH_Pin, GPIO_PIN_RESET);
}

void HAL_eps_ADCS_OFF() {
    HAL_GPIO_WritePin(GPIOC, GPIO_ADCS_SWITCH_Pin, GPIO_PIN_SET);
}

void HAL_eps_COMMS_ON() {
    HAL_GPIO_WritePin(GPIOH, GPIO_COMM_SWITCH_Pin, GPIO_PIN_RESET);
}

void HAL_eps_COMMS_OFF() {
    HAL_GPIO_WritePin(GPIOH, GPIO_COMM_SWITCH_Pin, GPIO_PIN_SET);
}

void HAL_eps_SU_ON() {
    HAL_GPIO_WritePin(GPIOC, GPIO_SU_SWITCH_Pin, GPIO_PIN_RESET);
}

void HAL_eps_SU_OFF() {
    HAL_GPIO_WritePin(GPIOC, GPIO_SU_SWITCH_Pin, GPIO_PIN_SET);
}

void HAL_uart_tx(TC_TM_app_id app_id, uint8_t *buf, uint16_t size) {
    
    HAL_StatusTypeDef res;
    UART_HandleTypeDef *huart;

    if(app_id == OBC_APP_ID) { huart = &huart3; }
    else if(app_id == DBG_APP_ID) { huart = &huart3; }

    //HAL_UART_Transmit(&huart2, buf, size, 10);
    for(;;) { // should use hard limits
        res = HAL_UART_Transmit_DMA(huart, buf, size);
        if(res == HAL_OK) { break; }
        HAL_Delay(10);
    }
}

SAT_returnState HAL_uart_rx(TC_TM_app_id app_id, struct uart_data *data) {

    UART_HandleTypeDef *huart;

    if(app_id == OBC_APP_ID) { huart = &huart3; }

    if((huart->State == HAL_UART_STATE_READY) || (huart->State == HAL_UART_STATE_BUSY_TX)) {
        data->uart_size = huart->RxXferSize - huart->RxXferCount;
        for(uint16_t i = 0; i < data->uart_size; i++) { data->uart_unpkt_buf[i] = data->uart_buf[i]; }
        HAL_UART_Receive_IT(huart, data->uart_buf, UART_BUF_SIZE);
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
void HAL_EPS_UART_IRQHandler(UART_HandleTypeDef *huart)
{
  uint32_t tmp_flag = 0, tmp_it_source = 0;

  tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE);
  tmp_it_source = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_RXNE);
  /* UART in mode Receiver ---------------------------------------------------*/
  if((tmp_flag != RESET) && (tmp_it_source != RESET))
  { 
    UART_EPS_Receive_IT(huart);
  }

}

/**
  * @brief  Receives an amount of data in non blocking mode 
  * @param  huart: pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
void UART_EPS_Receive_IT(UART_HandleTypeDef *huart)
{
  
    uint8_t c;

    c = (uint8_t)(huart->Instance->DR & (uint8_t)0x00FF);
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

      /* Check if a transmit process is ongoing or not */
      if(huart->State == HAL_UART_STATE_BUSY_TX_RX) 
      {
        huart->State = HAL_UART_STATE_BUSY_TX;
      }
      else
      {
        /* Disable the UART Parity Error Interrupt */
        __HAL_UART_DISABLE_IT(huart, UART_IT_PE);

        /* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
        __HAL_UART_DISABLE_IT(huart, UART_IT_ERR);

        huart->State = HAL_UART_STATE_READY;
      }
    } else if(huart->RxXferSize > huart->RxXferCount) {
      *huart->pRxBuffPtr++ = c;
      huart->RxXferCount--;
    }

    if(huart->RxXferCount == 0U) // errror
    {


    }

}

void HAL_reset_source(uint8_t *src) {

    *src |= (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) << 1);
    *src |= (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) << 2);
    *src |= (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST) << 3);
    *src |= (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) << 4);
    *src |= (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST) << 5);
    *src |= (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST) << 6);
 
    __HAL_RCC_CLEAR_RESET_FLAGS();

}

void HAL_sys_setTime(uint8_t hours, uint8_t mins, uint8_t sec) {

  RTC_TimeTypeDef sTime;

  sTime.Hours = hours;
  sTime.Minutes = mins;
  sTime.Seconds = sec;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);


}

void HAL_sys_getTime(uint8_t *hours, uint8_t *mins, uint8_t *sec) {

  RTC_TimeTypeDef sTime;

  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

   *hours = sTime.Hours;
   *mins = sTime.Minutes;
   *sec = sTime.Seconds;  
}

void HAL_sys_setDate(uint8_t mon, uint8_t date, uint8_t year) {

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

uint32_t HAL_sys_GetTick() {
  return HAL_GetTick();
}
