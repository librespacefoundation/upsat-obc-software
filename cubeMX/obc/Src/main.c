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
#include "service_utilities.h"
#include "time_management_service.h"
#include "wdg.h"
#include "su_mnlp.h"
#include "scheduling_service.h"


#undef __FILE_ID__
#define __FILE_ID__ 666

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

IWDG_HandleTypeDef hiwdg;

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
osThreadId hkHandle;
osThreadId idleHandle;
osThreadId su_schHandle;
osThreadId sche_servHandle;
osMessageQId queueCOMMS;
osMessageQId queueADCS;
osMessageQId queueDBG;
osMessageQId queueEPS;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t uart_temp[200];
extern uint8_t su_inc_resp[180]; //174 su resp + 22 flight hdr, +1 for serial shift = 197
extern struct _MNLP_data MNLP_data;

TaskHandle_t xTask_UART = NULL;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
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
static void MX_IWDG_Init(void);
void UART_task(void const * argument);
void HK_task(void const * argument);
void IDLE_task(void const * argument);
void SU_SCH(void const * argument);
void sche_se_sch(void const * argument);

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
  //MX_IWDG_Init();

  /* USER CODE BEGIN 2 */
  //wdg_INIT();
  // uart_temp[0] = key_test[4];

  SEGGER_SYSVIEW_Conf();
  sysview_init();

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
  osThreadDef(uart, UART_task, osPriorityAboveNormal, 0, 1024);
  uartHandle = osThreadCreate(osThread(uart), NULL);

  /* definition and creation of HK */
  osThreadDef(HK, HK_task, osPriorityLow, 0, 512);
  hkHandle = osThreadCreate(osThread(HK), NULL);

  /* definition and creation of time_check */
  osThreadDef(time_check, IDLE_task, osPriorityIdle, 0, 512);
  idleHandle = osThreadCreate(osThread(time_check), NULL);

  /* definition and creation of SU_SCH_task */
  osThreadDef(SU_SCH_task, SU_SCH, osPriorityNormal, 0, 512);
  su_schHandle = osThreadCreate(osThread(SU_SCH_task), NULL);

  /* definition and creation of scheduling_serv */
  osThreadDef(scheduling_serv, sche_se_sch, osPriorityBelowNormal, 0, 128);
  sche_servHandle = osThreadCreate(osThread(scheduling_serv), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the queue(s) */
  /* definition and creation of queueCOMMS */
  osMessageQDef(queueCOMMSdef, POOL_PKT_TOTAL_SIZE + 1, sizeof(tc_tm_pkt *));
  queueCOMMS = osMessageCreate(osMessageQ(queueCOMMSdef), NULL);

  /* definition and creation of queueADCS */
  osMessageQDef(queueADCSdef, POOL_PKT_TOTAL_SIZE + 1, sizeof(tc_tm_pkt *));
  queueADCS = osMessageCreate(osMessageQ(queueADCSdef), NULL);

  /* definition and creation of queueDBG */
  osMessageQDef(queueDBGdef, POOL_PKT_TOTAL_SIZE + 1, sizeof(tc_tm_pkt *));
  queueDBG = osMessageCreate(osMessageQ(queueDBGdef), NULL);

  /* definition and creation of queueEPS */
  osMessageQDef(queueEPSdef, POOL_PKT_TOTAL_SIZE + 1, sizeof(tc_tm_pkt *));
  queueEPS = osMessageCreate(osMessageQ(queueEPSdef), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  uint32_t test = uxQueueMessagesWaiting( queueEPS );
  test = uxQueueMessagesWaiting( queueDBG );
  test = uxQueueMessagesWaiting( queueADCS );
  test = uxQueueMessagesWaiting( queueCOMMS );
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

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE
                              |RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 224;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/* ADC1 init function */
static void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_VBAT;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

/* IWDG init function */
static void MX_IWDG_Init(void)
{

  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }

}

/* RTC init function */
static void MX_RTC_Init(void)
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
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  sTime.Hours = 0;
  sTime.Minutes = 0;
  sTime.Seconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  //if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  //{
  //  Error_Handler();
  //}

  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 1;
  sDate.Year = 0;

  //if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  //{
  //  Error_Handler();
  //}

    /**Enable Calibrartion 
    */
  //if (HAL_RTCEx_SetCalibrationOutPut(&hrtc, RTC_CALIBOUTPUT_1HZ) != HAL_OK)
  //{
  //  Error_Handler();
  //}

}

/* SDIO init function */
static void MX_SDIO_SD_Init(void)
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
static void MX_SPI1_Init(void)
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
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }

}

/* SPI2 init function */
static void MX_SPI2_Init(void)
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
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }

}

/* SPI3 init function */
static void MX_SPI3_Init(void)
{

  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_SLAVE;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }

}

/* UART4 init function */
static void MX_UART4_Init(void)
{

  huart4.Instance = UART4;
  huart4.Init.BaudRate = 9600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

}

/* USART3 init function */
static void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }

}

/* USART6 init function */
static void MX_USART6_UART_Init(void)
{

  huart6.Instance = USART6;
  huart6.Init.BaudRate = 9600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
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
static void MX_GPIO_Init(void)
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

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
  
  //if(hspi == &hspi3) {
    obc_data.iac_flag = true; 
  
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(xTask_UART, &xHigherPriorityTaskWoken);
  //}
}

void HAL_SPI_ErrorCallback (SPI_HandleTypeDef * hspi) {

  //if(hspi == &hspi3) {

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(xTask_UART, &xHigherPriorityTaskWoken);
  //}

}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    obc_data.adc_flag = true;
}


/* USER CODE END 4 */

/* UART_task function */
void UART_task(void const * argument)
{
  /* init code for FATFS */
  MX_FATFS_Init();

  /* USER CODE BEGIN 5 */
  
   //obc_data.rsrc = 0;

   /*IS25LP128  eeprom*/
   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
   
   pkt_pool_INIT();
   HAL_obc_enableBkUpAccess();
   bkup_sram_INIT();

   uint8_t rsrc = 0;
   HAL_reset_source(&rsrc);
   set_reset_source(rsrc);

   uint32_t b_cnt = 0;
   get_boot_counter(&b_cnt);
   event_boot(rsrc, b_cnt);

   update_boot_counter();

   HAL_obc_IAC_ON();
   
   HAL_obc_SD_ON();
   
   mass_storage_init();

   su_INIT();

   scheduling_service_init();
   
  /*Task notification setup*/
  uint32_t ulNotificationValue;
  const TickType_t xMaxBlockTime = pdMS_TO_TICKS(10000);

  xTask_UART = xTaskGetCurrentTaskHandle();

  //HAL_SPI_TransmitReceive_IT(&hspi3, obc_data.iac_out, obc_data.iac_in, 16);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
  osDelay(1);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
  /*Uart inits*/
  HAL_UART_Receive_IT( &huart1, obc_data.eps_uart.uart_buf, UART_BUF_SIZE);
  HAL_UART_Receive_IT( &huart2, MNLP_data.su_inc_resp, 174);
  HAL_UART_Receive_IT( &huart3, obc_data.dbg_uart.uart_buf, UART_BUF_SIZE);
  HAL_UART_Receive_IT( &huart4, obc_data.comms_uart.uart_buf, UART_BUF_SIZE);
  HAL_UART_Receive_IT( &huart6, obc_data.adcs_uart.uart_buf, UART_BUF_SIZE);

  /* Infinite loop */
  for(;;)
  {
    task_times.uart_time = HAL_sys_GetTick();
    su_incoming_rx();
    import_pkt(EPS_APP_ID, &obc_data.eps_uart);
    import_pkt(DBG_APP_ID, &obc_data.dbg_uart);
    import_pkt(COMMS_APP_ID, &obc_data.comms_uart);
    import_pkt(ADCS_APP_ID, &obc_data.adcs_uart);
    import_spi();

    export_pkt(EPS_APP_ID, &obc_data.eps_uart);
    export_pkt(ADCS_APP_ID, &obc_data.adcs_uart);
    export_pkt(COMMS_APP_ID, &obc_data.comms_uart);
    export_pkt(DBG_APP_ID, &obc_data.dbg_uart);

    wdg.uart_valid = true;
    ulNotificationValue = ulTaskNotifyTake( pdTRUE, xMaxBlockTime);
    
  }
  /* USER CODE END 5 */ 
}

/* HK_task function */
void HK_task(void const * argument)
{
  /* USER CODE BEGIN HK_task */
  hk_INIT();
  /* Infinite loop */
  osDelay(6000);
  for(;;)
  {
    task_times.hk_time = HAL_sys_GetTick();
    hk_SCH();
    osDelay(10);
  }
  /* USER CODE END HK_task */
}

/* IDLE_task function */
void IDLE_task(void const * argument)
{
  /* USER CODE BEGIN IDLE_task */
   
    /*Task notification setup*/
  struct time_utc utc;
  uint32_t qb_secs;
  osDelay(5000);
  /* Infinite loop */
  for(;;)
  {
    uint32_t time = HAL_sys_GetTick();
    task_times.idle_time = time;

//    uart_killer(EPS_APP_ID, &obc_data.eps_uart, time);
//    uart_killer(DBG_APP_ID, &obc_data.dbg_uart, time);
//    uart_killer(COMMS_APP_ID, &obc_data.comms_uart, time);
//    uart_killer(ADCS_APP_ID, &obc_data.adcs_uart, time);

    if(time - obc_data.adc_time > 30000) {
      HAL_ADC_Start_IT(&hadc1);
      obc_data.adc_time = time;
    }
      
    if(obc_data.adc_flag == true) {
      obc_data.vbat = (uint16_t) HAL_ADC_GetValue(&hadc1);
      HAL_ADC_Stop_IT(&hadc1);
      obc_data.adc_flag = false;
    }
    
    pkt_pool_IDLE();
    osDelay(1000);
    
  }
  /* USER CODE END IDLE_task */
}

/* SU_SCH function */
void SU_SCH(void const * argument)
{
  /* USER CODE BEGIN su_sch_task */
  uint32_t sleep_val_secs=55;
  su_mnlp_returnState su_sche_state = su_sche_last;
  osDelay(5000);
  /* Infinite loop */
  for (;;){
    if( *MNLP_data.su_service_sheduler_active){
          
//        tc_tm_pkt *su_temp = get_pkt(PKT_NORMAL);
//        hk_crt_pkt_TC( su_temp, ADCS_APP_ID, SU_SCI_HDR_REP);
//        route_pkt(su_temp);
        
        sleep_val_secs = 0;
        su_sche_state = su_script_selector(&sleep_val_secs);
//        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS((sleep_val_secs)*1000));
        if( su_sche_state == su_no_scr_eligible){
            ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS((sleep_val_secs)*1000)); /*sleeps for <50 secs*/
            continue;
        }
        else
        if( su_sche_state == su_new_scr_selected){ /*script marked active 1 to 60 seconds earlier, sleep this time*/
            ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS((sleep_val_secs)*1000));
            continue;
            //run the su_SCH?
        }
        else
        if( su_sche_state == su_no_new_scr_selected){ /*so there is an already active script, old or new, go for it*/
            if( (*MNLP_data.su_nmlp_script_scheduler_active) == (uint8_t) true){
                sleep_val_secs = 0;
                su_sche_state = su_SCH(&sleep_val_secs);                
                if( su_sche_state == su_sche_script_ended){
                    /*all time tables inside su_SCH has been served. Select a new script or Go for the next science collection day*/
                   //TODO: disable the scheduler ?
                   ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS((sleep_val_secs)*1000));
                   continue;
                   /*TODO: also to be notified on script upload*/
                }
            }
        }
      }
    osDelay(2000);
  }  
  /* USER CODE END su_sch_task */
}

/* sche_se_sch function */
void sche_se_sch(void const * argument)
{
  /* USER CODE BEGIN sche_se_sch */
  /* Infinite loop */
  for(;;)
  {
    task_times.sch_time = HAL_sys_GetTick();
    cross_schedules();
    osDelay(1000);
  }
  /* USER CODE END StartTask05 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
/* USER CODE BEGIN Callback 0 */

/* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
/* USER CODE BEGIN Callback 1 */

/* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
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
