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
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_uart.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "scheduling_service.h"
//#include "circular_buffer.h"
//#include "scheduling_service.h"
//#include "time_management.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
__IO ITStatus UartReady = SET;

/* Buffer used for reception */
uint8_t total_buffer[RXBUFFERSIZE];
//extern CircQueue total_buffer;

/* Buffer used for transmission */
//uint8_t aTxBuffer[] = " ****UART_TwoBoards_ComIT****";

osThreadId defaultTaskHandle;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

TaskFunction_t ToggleLED_Timer1(void*);
TaskFunction_t ToggleLED_Timer2(void*);
TaskFunction_t ToggleLED_Timer3(void*);
TaskFunction_t ToggleLED_Timer4(void*);
TaskFunction_t UART2_ReceiveData(void*);
TaskFunction_t UART2_TransmitData(void*);

void MX_UART2_UART_Init(void);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
//extern void Error_Handler(void);
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
static void MX_GPIO_Init(void);
void StartDefaultTask(void const * argument);

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle);
//int putcharx(int ch)
//{
//    while (__HAL_UART_GET_FLAG(USART2, USART_FLAG_TXE) == RESET);
//    USART_SendData(USART2, (uint8_t)ch);
//    return ch;
//}
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
    MX_UART2_UART_Init();
      
    osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

    /*Buffers initializations*/
    memset(&total_buffer[0],0,sizeof(total_buffer));
//    while(1){}
    /* Create tasks */
//    xTaskCreate(
//                UART2_ReceiveData, /* Function pointer */
//                "usasrtrx", /* Task name - for debugging only*/
//                configMINIMAL_STACK_SIZE, /* Stack depth in words */
//                (void*) NULL, /* Pointer to tasks arguments (parameter) */
//                tskIDLE_PRIORITY + 2UL, /* Task priority*/
//                NULL /* Task handle */
//                );

//    xTaskCreate(
//                UART2_TransmitData, /* Function pointer */
//                "usasrtrx", /* Task name - for debugging only*/
//                configMINIMAL_STACK_SIZE, /* Stack depth in words */
//                (void*) NULL, /* Pointer to tasks arguments (parameter) */
//                tskIDLE_PRIORITY + 2UL, /* Task priority*/
//                NULL /* Task handle */
//                );
//       xTaskCreate(
//                init_and_run_time, /* Function pointer */
//                "timing", /* Task name - for debugging only*/
//                configMINIMAL_STACK_SIZE, /* Stack depth in words */
//                (void*) NULL, /* Pointer to tasks arguments (parameter) */
//                tskIDLE_PRIORITY + 2UL, /* Task priority*/
//                NULL /* Task handle */
//                );
       
//       xTaskCreate(
//                cross_schedules, /* Function pointer */
//                "scheduling", /* Task name - for debugging only*/
//                configMINIMAL_STACK_SIZE, /* Stack depth in words */
//                (void*) NULL, /* Pointer to tasks arguments (parameter) */
//                tskIDLE_PRIORITY + 2UL, /* Task priority*/
//                NULL /* Task handle */
//                );
       
//      xTaskCreate(
//               ToggleLED_Timer1, /* Function pointer */
//               "Task1", /* Task name - for debugging only*/
//               configMINIMAL_STACK_SIZE, /* Stack depth in words */
//               (void*) NULL, /* Pointer to tasks arguments (parameter) */
//               tskIDLE_PRIORITY + 2UL, /* Task priority*/
//               NULL /* Task handle */
//      );
//
//      xTaskCreate(
//    		  ToggleLED_Timer2,                 /* Function pointer */
//    		  "Task1",                          /* Task name - for debugging only*/
//    		  configMINIMAL_STACK_SIZE,         /* Stack depth in words */
//    		  (void*) NULL,                     /* Pointer to tasks arguments (parameter) */
//    		  tskIDLE_PRIORITY + 2UL,           /* Task priority*/
//    		  NULL                              /* Task handle */
//      );
//      
//      xTaskCreate(
//    		  ToggleLED_Timer3,                 /* Function pointer */
//    		  "Task1",                          /* Task name - for debugging only*/
//    		  configMINIMAL_STACK_SIZE,         /* Stack depth in words */
//    		  (void*) NULL,                     /* Pointer to tasks arguments (parameter) */
//    		  tskIDLE_PRIORITY + 2UL,           /* Task priority*/
//    		  NULL                              /* Task handle */
//      );
//      
//      xTaskCreate(
//    		  ToggleLED_Timer4,                 /* Function pointer */
//    		  "Task1",                          /* Task name - for debugging only*/
//    		  configMINIMAL_STACK_SIZE,         /* Stack depth in words */
//    		  (void*) NULL,                     /* Pointer to tasks arguments (parameter) */
//    		  tskIDLE_PRIORITY + 2UL,           /* Task priority*/
//    		  NULL                              /* Task handle */
//      );

      
//      service_init();
//      HAL_UART_Transmit(&Uart2Handle, (uint8_t *)"Init", 25,5000);
    /* Start scheduler */
    //this is the final point for linear code.
    osKernelStart();
    
//            if(HAL_UART_Receive_IT(&Uart2Handle, (uint8_t *)total_buffer, 455) != HAL_OK)
//            {
//              Error_Handler();
//            }
//            if(HAL_UART_Transmit_IT(&Uart2Handle, (uint8_t *)total_buffer, 455) != HAL_OK)
//            {
//              Error_Handler();
//            }
//            if(HAL_UART_Receive(&Uart2Handle, (uint8_t *)total_buffer, 455,5000) != HAL_OK)
//            {
//              Error_Handler();
//            }
//            if(HAL_UART_Transmit(&Uart2Handle, (uint8_t *)total_buffer, 455,5000) != HAL_OK)
//            {
//              Error_Handler();
//            }
            
        /*  We should never get here as control is now taken by the scheduler */

        /* Infinite loop */
        /* USER CODE BEGIN WHILE */

        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */

    
        /* USER CODE END 3 */

}//main ends here

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    /* Set transmission flag: transfer complete */
    /* Turn LED6 on: Transfer in transmission process is correct */
//    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12,GPIO_PIN_SET);
//    UartReady = SET;
}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of IT Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    /* Set transmission flag: transfer complete */
//    __HAL_UART_FLUSH_DRREGISTER(UartHandle);
//    UartReady = SET;
//    HAL_UART_Receive_IT(&Uart2Handle, (uint8_t *)total_buffer, 1);
    /* Turn LED4 on: Transfer in reception process is correct */
//    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13,GPIO_PIN_SET);
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
 void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
//     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14,GPIO_PIN_SET); 
//     UartReady = SET;
//     __HAL_UART_DISABLE_IT(UartHandle, UART_IT_ERR);
//        UartHandle->State = HAL_UART_STATE_READY;
//     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12,GPIO_PIN_RESET); 
//    /* Turn LED3 on: Transfer error in reception/transmission process */
//    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12,GPIO_PIN_SET); 
}

//void USART2_IRQHandler(void)
//{
//}
 
TaskFunction_t UART2_ReceiveData(void* pvParameters)
{
    while (1)
    {
        HAL_UART_Receive(&huart2, (uint8_t *) total_buffer, RXBUFFERSIZE-1, 5000);
        for ( int u=0;u<RXBUFFERSIZE;u++)
        {
            HAL_UART_Transmit(&huart2, (uint8_t *) total_buffer, RXBUFFERSIZE-1, 5000);
        }

        vTaskDelay(250 / portTICK_RATE_MS);
    }
}

TaskFunction_t UART2_TransmitData(void* pvParameters)
{
    while (1)
    {
        char *test = "Hello\n";
        
        //        HAL_UART_Receive_IT( &UartHandle, gpsdata ,100 );
        HAL_UART_Transmit(&huart2, (uint8_t *) test, RXBUFFERSIZE-1, 5000);
        
        vTaskDelay(250 / portTICK_RATE_MS);
    }
}

/**
 * TASK 1: Toggle LED via RTOS Timer
 */
TaskFunction_t ToggleLED_Timer1(void *pvParameters)
{
    while (1)
    {
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
        //    GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
        //    GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
        //    GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
        /*
        Delay for a period of time. vTaskDelay() places the task into
        the Blocked state until the period has expired.
        The delay period is spacified in 'ticks'. We can convert
        yhis in milisecond with the constant portTICK_RATE_MS.
         */
        vTaskDelay(250 / portTICK_RATE_MS);
    }

}

/**
 * TASK 1: Toggle LED via RTOS Timer
 */
TaskFunction_t ToggleLED_Timer2(void *pvParameters)
{
    while (1)
    {
        //    GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
        //    GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
        //    GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
        /*
        Delay for a period of time. vTaskDelay() places the task into
        the Blocked state until the period has expired.
        The delay period is spacified in 'ticks'. We can convert
        yhis in milisecond with the constant portTICK_RATE_MS.
         */
        vTaskDelay(500 / portTICK_RATE_MS);
    }
}

/**
 * TASK 1: Toggle LED via RTOS Timer
 */
TaskFunction_t ToggleLED_Timer3(void *pvParameters)
{
    while (1)
    {
        //    GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
        //    GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
        //    GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
        /*
        Delay for a period of time. vTaskDelay() places the task into
        the Blocked state until the period has expired.
        The delay period is spacified in 'ticks'. We can convert
        yhis in milisecond with the constant portTICK_RATE_MS.
         */
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

/**
 * TASK 1: Toggle LED via RTOS Timer
 */
TaskFunction_t ToggleLED_Timer4(void *pvParameters)
{
    while (1)
    {
        //    GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
        //    GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
        //    GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
        /*
        Delay for a period of time. vTaskDelay() places the task into
        the Blocked state until the period has expired.
        The delay period is spacified in 'ticks'. We can convert
        yhis in milisecond with the constant portTICK_RATE_MS.
         */
        vTaskDelay(1500 / portTICK_RATE_MS);
    }
}

/** System Clock Configuration
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    __PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 16;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1
            | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV8;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
    if (HAL_GetREVID() == 0x1001)
    {
      /* Enable the Flash prefetch */
      __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
    }

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
//    SysTick_Config(SystemCoreClock/1000);
//    HAL_NVIC_EnableIRQ(SysTick_IRQn);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */

/* USART2 init function */
void MX_UART2_UART_Init(void)
{
    /*##-1- Configure the UART peripheral ######################################*/
    /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
    /* UART1 configured as follow:
        - Word Length = 8 Bits
        - Stop Bit = One Stop bit
        - Parity = None
        - BaudRate = 9600 baud
        - Hardware flow control disabled (RTS and CTS signals) */
    huart2.Instance = USARTx;
    
    huart2.Init.BaudRate = 9600;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    
    if(HAL_UART_Init(&huart2) != HAL_OK)
    {
//      Error_Handler();
    }
    
}

void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO Ports Clock Enable */
    __GPIOE_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();
    __GPIOH_CLK_ENABLE();
    __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
    __GPIOD_CLK_ENABLE();

    __USART2_CLK_ENABLE();
    __USART1_CLK_ENABLE();


    /*Configure GPIO pin : CS_I2C_SPI_Pin */
    GPIO_InitStruct.Pin = CS_I2C_SPI_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
    GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : PDM_OUT_Pin */
    GPIO_InitStruct.Pin = PDM_OUT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(PDM_OUT_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : B1_Pin */
    GPIO_InitStruct.Pin = B1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : PA4 */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : PA5 PA6 PA7 */
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : BOOT1_Pin */
    GPIO_InitStruct.Pin = BOOT1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : CLK_IN_Pin */
    GPIO_InitStruct.Pin = CLK_IN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(CLK_IN_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin 
                             Audio_RST_Pin */
    GPIO_InitStruct.Pin = LD4_Pin | LD3_Pin | LD5_Pin | LD6_Pin
            | Audio_RST_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pins : PC7 I2S3_SCK_Pin PC12 */
    GPIO_InitStruct.Pin = GPIO_PIN_7 | I2S3_SCK_Pin | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin : VBUS_FS_Pin */
    GPIO_InitStruct.Pin = VBUS_FS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(VBUS_FS_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : OTG_FS_ID_Pin OTG_FS_DM_Pin OTG_FS_DP_Pin */
    GPIO_InitStruct.Pin = OTG_FS_ID_Pin | OTG_FS_DM_Pin | OTG_FS_DP_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
    GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : Audio_SCL_Pin Audio_SDA_Pin */
    GPIO_InitStruct.Pin = Audio_SCL_Pin | Audio_SDA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pin : MEMS_INT2_Pin */
    GPIO_InitStruct.Pin = MEMS_INT2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(MEMS_INT2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* StartDefaultTask function */

//uint8_t t[19] = {126,24,1,192,185,0,10,16,8,1, 6,1,0,0,0,8,0,124,126};
//
//char *t2[2] = {126,24};
//char *temp = t2;

void StartDefaultTask(void const * argument)
{
    /* init code for FATFS */
    MX_FATFS_Init();

    /* USER CODE BEGIN 5 */
    uint8_t uart_temp[10];
    pkt_pool_INIT();
//    mass_storage_init();
    
//    sprintf((char*)uart_temp, t);
    sprintf((char*)uart_temp, "Hello\n");
//    for(;;){
        HAL_UART_Transmit(&huart2, uart_temp, 6 , 1000);
//        HAL_Delay(2000);
//    }
    /* Infinite loop */
             
    scheduling_init_service();
//    load_schedules();
//    cross_schedules();
    
    for(;;)
    {   
//        C_ASSERT(false);
        import_eps_pkt();
        cross_schedules();
        osDelay(1);
    }
    /* USER CODE END 5 */ 
}

/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle. 
  * @note   This example shows a simple way to report end of IT Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */


#ifdef USE_FULL_ASSERT

/**
 * @brief Reports the name of the source file and the source line number
 * where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval None
 */
void
assert_failed(uint8_t* file, uint32_t line)
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
