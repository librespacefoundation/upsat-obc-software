/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "fatfs.h"

/* USER CODE BEGIN Includes */
#include "obc.h"
#include "su_mnlp.h"
#include "service_utilities.h"

#undef __FILE_ID__
#define __FILE_ID__ 666

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

RTC_HandleTypeDef hrtc;

SD_HandleTypeDef hsd;
HAL_SD_CardInfoTypedef SDCardInfo;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi3;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_uart4_tx;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart3_tx;
DMA_HandleTypeDef hdma_usart6_tx;

osThreadId uartHandle;
osThreadId HKHandle;
osThreadId time_checkHandle;
osThreadId SU_SCH_taskHandle;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t uart_temp[200];
extern uint8_t su_inc_buffer[210];
extern struct _MNLP_data MNLP_data;

TaskHandle_t xTask_UART = NULL;
TaskHandle_t xTask_IDLE = NULL;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SDIO_SD_Init(void);
static void MX_UART4_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SPI2_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI3_Init(void);
static void MX_ADC1_Init(void);
static void MX_RTC_Init(void);
void UART_task(void const * argument);
void HK_task(void const * argument);
void IDLE_task(void const * argument);
void SU_SCH(void const * argument);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SDIO_SD_Init();
  MX_UART4_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_USART1_UART_Init();
  MX_SPI2_Init();
  MX_SPI1_Init();
  MX_SPI3_Init();
  MX_ADC1_Init();
  MX_RTC_Init();

  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of uart */
  osThreadDef(uart, UART_task, osPriorityNormal, 0, 800);
  uartHandle = osThreadCreate(osThread(uart), NULL);

  /* definition and creation of HK */
  osThreadDef(HK, HK_task, osPriorityLow, 0, 128);
  HKHandle = osThreadCreate(osThread(HK), NULL);

  /* definition and creation of time_check */
  osThreadDef(time_check, IDLE_task, osPriorityIdle, 0, 128);
  time_checkHandle = osThreadCreate(osThread(time_check), NULL);

  /* definition and creation of SU_SCH_task */
  osThreadDef(SU_SCH_task, SU_SCH, osPriorityNormal, 0, 128);
  SU_SCH_taskHandle = osThreadCreate(osThread(SU_SCH_task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
 

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 224;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  HAL_ADC_Init(&hadc1);

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

}

/* RTC init function */
void MX_RTC_Init(void)
{

  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;

    /**Initialize RTC and set the Time and Date 
    */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  HAL_RTC_Init(&hrtc);

//  sTime.Hours = 0;
//  sTime.Minutes = 0;
//  sTime.Seconds = 0;
//  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
//  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  //HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

//  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
//  sDate.Month = RTC_MONTH_JANUARY;
//  sDate.Date = 1;
//  sDate.Year = 0;

  //HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

    /**Enable Calibrartion 
    */
  HAL_RTCEx_SetCalibrationOutPut(&hrtc, RTC_CALIBOUTPUT_1HZ);

}

/* SDIO init function */
void MX_SDIO_SD_Init(void)
{

  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 0;

}

/* SPI1 init function */
void MX_SPI1_Init(void)
{

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  HAL_SPI_Init(&hspi1);

}

/* SPI2 init function */
void MX_SPI2_Init(void)
{

  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  HAL_SPI_Init(&hspi2);

}

/* SPI3 init function */
void MX_SPI3_Init(void)
{

  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 10;
  HAL_SPI_Init(&hspi3);

}

/* UART4 init function */
void MX_UART4_Init(void)
{

  huart4.Instance = UART4;
  huart4.Init.BaudRate = 9600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart4);

}

/* USART1 init function */
void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart1);

}

/* USART2 init function */
void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart2);

}

/* USART3 init function */
void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart3);

}

/* USART6 init function */
void MX_USART6_UART_Init(void)
{

  huart6.Instance = USART6;
  huart6.Init.BaudRate = 9600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart6);

}

/** 
  * Enable DMA controller clock
  */
void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
  /* DMA1_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
  /* DMA2_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
  /* DMA2_Stream7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, COMMS_EN_Pin|ADC_CS_SPI1_Pin|IAC_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1|FLASH_HOLD_Pin|IAC_CS_SPI3_Pin|DBG_EN_Pin 
                          |IAC_CAMERA_PWR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, FLASH_WP_Pin|FLASH_CS_SPI2_Pin|SD_PWR_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : COMMS_EN_Pin ADC_CS_SPI1_Pin IAC_EN_Pin */
  GPIO_InitStruct.Pin = COMMS_EN_Pin|ADC_CS_SPI1_Pin|IAC_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB1 FLASH_HOLD_Pin IAC_CS_SPI3_Pin DBG_EN_Pin 
                           IAC_CAMERA_PWR_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_1|FLASH_HOLD_Pin|IAC_CS_SPI3_Pin|DBG_EN_Pin 
                          |IAC_CAMERA_PWR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : FLASH_WP_Pin FLASH_CS_SPI2_Pin SD_PWR_EN_Pin */
  GPIO_InitStruct.Pin = FLASH_WP_Pin|FLASH_CS_SPI2_Pin|SD_PWR_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* UART_task function */
void UART_task(void const * argument)
{
  /* init code for FATFS */
  MX_FATFS_Init();

  /* USER CODE BEGIN 5 */
    //obc_data.rsrc = 0;
   HAL_reset_source(&sys_data.rsrc);
   update_boot_counter();

   /*IS25LP128  eeprom*/
   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
   
   //uint32_t t1, t2, t3;
   
   //t1 = time_cmp_elapsed(3, 6);
   //t2 = time_cmp_elapsed(0xfffffff0, 0xfffffff6);
   //t3 = time_cmp_elapsed(0xfffffff0, 3);
   
   //t1 = get_time_ELAPSED();
   //osDelay(10);
   //t2 = get_time_ELAPSED();
   //osDelay(1000);
   //t3 = get_time_ELAPSED();
   
   //event_log(reset source);

   //if(!C_ASSERT(false) == true)
   pkt_pool_INIT();
   HAL_obc_enableBkUpAccess();
   bkup_sram_INIT();
   
   HAL_obc_IAC_ON();
   
   HAL_obc_SD_ON();
   
   mass_storage_init();
   
   large_data_INIT();
   
   su_INIT();
   
   //uint32_t papartime = time_now();
  
   //uint8_t g = 5;
   
   //uint8_t hours, mins, sec = 0;
   //HAL_obc_getTime(&hours, &mins, &sec);
   //sprintf((char*)uart_temp, "T: %d:%d.%d\n", hours, mins, sec);
   //HAL_UART_Transmit(&huart2, uart_temp, 19 , 10000);
   
   //hours = 19;
   //mins = 35;
   //sec = 11;
   //HAL_obc_setTime(hours, mins, sec);
   //sprintf((char*)uart_temp, "T: %d:%d.%d\n", hours, mins, sec);
   //HAL_UART_Transmit(&huart2, uart_temp, 19 , 10000);
   
   //sprintf((char*)uart_temp, "F: %u\n", *obc_data.log_cnt);
   //HAL_UART_Transmit(&huart2, uart_temp, 19 , 10000);
   //(*obc_data.log_cnt)++;
   //*obc_data.log_cnt = 0;
   //uint8_t tt[] = "YO!\n";
   //event_log(tt, 4);

   //event_log_load(uart_temp, (*obc_data.log_cnt) - 4, 4);
   //HAL_UART_Transmit(&huart2, uart_temp, 5 , 10000);
   
   //sprintf((char*)uart_temp, "\nR: %02x\n", obc_data.rsrc);
   //HAL_UART_Transmit(&huart2, uart_temp, 19 , 10000);
//   uint8_t spi_in_temp[7], spi_out_temp[7];
//   
//   /*IS25LP128  eeprom*/
//   spi_in_temp[0] = 0x90;
//   spi_in_temp[1] = 0x00;
//   spi_in_temp[2] = 0x00;
//   spi_in_temp[3] = 0x00;
//   spi_in_temp[4] = 0x00;
//   spi_in_temp[5] = 0x00;
//   spi_in_temp[6] = 0x00;
   
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
//    HAL_SPI_TransmitReceive(&hspi2, spi_in_temp, spi_out_temp, 7, 10000);
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
//    //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
//    sprintf(uart_temp, "IS25LP128 %d %d %d %d %d %d %d\n", spi_out_temp[0], spi_out_temp[1], spi_out_temp[2], spi_out_temp[3], spi_out_temp[4], spi_out_temp[5], spi_out_temp[6]);
//    HAL_UART_Transmit(&huart3, uart_temp, 30 , 10000);
//    HAL_UART_Transmit(&huart2, uart_temp, 30 , 10000);

//    for(uint8_t i = 0; i < 10; i++) {
//      /*AD7682*/
//      spi_in_temp[0] = 0xFA; //0b11110001;
//      spi_in_temp[1] = 0x40; //0b00000100;
//      spi_in_temp[2] = 0x00;
//      spi_in_temp[3] = 0x00;
//      spi_in_temp[4] = 0x00;
//      spi_in_temp[5] = 0x00;
//      
//      spi_out_temp[0] = 0x00;
//      spi_out_temp[1] = 0x00; 
//      spi_out_temp[2] = 0x00;
//      spi_out_temp[3] = 0x00;
//      spi_out_temp[4] = 0x00;
//      spi_out_temp[5] = 0x00;
//      
//      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
//      osDelay(1);
//      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
//      osDelay(1);
//      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
//      osDelay(1);
//      HAL_SPI_TransmitReceive(&hspi1, spi_in_temp, spi_out_temp, 4, 100);
//      //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
//      sprintf(uart_temp, "AD7682 %d %d %d %d %d %d\n", spi_out_temp[0], spi_out_temp[1], spi_out_temp[2], spi_out_temp[3], spi_out_temp[4], spi_out_temp[5]);
//      HAL_UART_Transmit(&huart3, uart_temp, 29 , 10000);
//      osDelay(5);
//    }
      
    /*RTC*/
//    struct time_utc utc;
//    get_time_UTC(&utc);
//    sprintf(uart_temp, "TIME: Y:%d M:%d D:%d H:%d M:%d S:%d\n", utc.year, utc.month, utc.day, utc.hour, utc.min, utc.sec);
//    HAL_UART_Transmit(&huart3, uart_temp, 30 , 10000);
    
  sprintf((char*)uart_temp, "Hello\n");
  //HAL_UART_Transmit(&huart2, uart_temp, 6 , 10000);
  HAL_UART_Transmit(&huart3, uart_temp, 6 , 10000);
  HAL_UART_Transmit(&huart6, uart_temp, 6 , 10000);
  
  uint16_t size = 0;
  
  event_crt_pkt_api(uart_temp, "OBC STARTED", 666, 666, "", &size, SATR_OK);
  HAL_uart_tx(DBG_APP_ID, (uint8_t *)uart_temp, size);
  
  /*Task notification setup*/
  uint32_t ulNotificationValue;
  const TickType_t xMaxBlockTime = pdMS_TO_TICKS(10000);

  xTask_UART = xTaskGetCurrentTaskHandle();

  /*Uart inits*/
  HAL_UART_Receive_IT( &huart1, obc_data.eps_uart.uart_buf, UART_BUF_SIZE);
  HAL_UART_Receive_IT( &huart2,  &su_inc_buffer[21], 173);//&23,174
//  HAL_UART_Receive_IT( &huart2,  &su_inc_buffer[23], 174);
  HAL_UART_Receive_IT( &huart3, obc_data.dbg_uart.uart_buf, UART_BUF_SIZE);
  HAL_UART_Receive_IT( &huart4, obc_data.comms_uart.uart_buf, UART_BUF_SIZE);
  HAL_UART_Receive_IT( &huart6, obc_data.adcs_uart.uart_buf, UART_BUF_SIZE);
//  HAL_UART_Receive_IT( &huart6, obc_data.su_uart.uart_buf, UART_BUF_SIZE);
  
   //temporal su sim test code

//   uint8_t su_out[200];
//
//          while(true){
//
//          su_out[0]= 0xF1;
//
//          su_out[1]= 1;
//
//          su_out[2]= 1;
//
//          HAL_UART_Transmit( &huart2, su_out, 3 , 10); //ver ok
//
//          su_out[0]= 0x05;
//
//          su_out[1]= 0x63; //len
//
//          su_out[2]= 2; //seq_coun
//
//          HAL_UART_Transmit( &huart2, su_out, 102 , 10); //ver ok
//          }
          
          
  /* Infinite loop */
  for(;;)
  {
    su_incoming_rx();
    import_pkt(EPS_APP_ID, &obc_data.eps_uart);
    import_pkt(DBG_APP_ID, &obc_data.dbg_uart);
    import_pkt(COMMS_APP_ID, &obc_data.comms_uart);
    import_pkt(ADCS_APP_ID, &obc_data.adcs_uart);
//    import_pkt(SU_APID, &obc_data.su_uart);
    
    
    ulNotificationValue = ulTaskNotifyTake( pdTRUE, xMaxBlockTime);
    
    //sprintf((char*)uart_temp, "Task %d\n", ulNotificationValue);
    //HAL_UART_Transmit(&huart3, uart_temp, strlen(uart_temp) , 10000);
    //osDelay(1);
  }
  /* USER CODE END 5 */ 
}

/* HK_task function */
void HK_task(void const * argument)
{
  /* USER CODE BEGIN HK_task */
  hk_INIT();
  /* Infinite loop */
 
  for(;;)
  {
    hk_SCH();
//    struct time_utc utc;
//    get_time_UTC(&utc);
//    sprintf(uart_temp, "TIME: Y:%d M:%d D:%d H:%d M:%d S:%d\n", utc.year, utc.month, utc.day, utc.hour, utc.min, utc.sec);
//    HAL_UART_Transmit(&huart3, uart_temp, 50 , 10000);
    osDelay(10);
  }
  /* USER CODE END HK_task */
}

/* IDLE_task function */
void IDLE_task(void const * argument)
{
  /* USER CODE BEGIN IDLE_task */
    
    /*Task notification setup*/
//  uint32_t ulNotificationValue;
//  const TickType_t xMaxBlockTime = pdMS_TO_TICKS(10000);
//  xTask_IDLE = xTaskGetCurrentTaskHandle();
  
  /* Infinite loop */
  for(;;)
  {
//    check_timeouts();
//    ulNotificationValue = ulTaskNotifyTake( pdTRUE, xMaxBlockTime);
    osDelay(100);
  }
  /* USER CODE END IDLE_task */
}

/* SU_SCH function */
void SU_SCH(void const * argument)
{
  /* USER CODE BEGIN SU_SCH */
  /* Infinite loop */
    
//    uint8_t su_out[200];
//
//    while(true){
//
//    su_out[0]= 0xF1;
//
//    su_out[1]= 1;
//
//    su_out[2]= 1;
//
//    HAL_UART_Transmit( &huart2, su_out, 3 , 10); //ver ok
//
//    su_out[0]= 0x05;
//
//    su_out[1]= 0x63; //len
//
//    su_out[2]= 2; //seq_coun
//
//    HAL_UART_Transmit( &huart2, su_out, 102 , 10); //ver ok
//    
//    su_out[0]= 0xF2;
//
//    su_out[1]= 1;
//
//    su_out[2]= 1;
//    
//    HAL_UART_Transmit( &huart2, su_out, 3 , 10); //ver ok
//}
    
  for(;;)
  {
      if( MNLP_data.su_nmlp_sche_active == true){
          su_SCH();
      }
      else{ osDelay(100); }
  }
  /* USER CODE END SU_SCH */
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
