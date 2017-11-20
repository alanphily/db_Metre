/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "../LIBS/WS2813.h"
#include "dfsdm.h"
#include "math.h"
#include "usart.h"

//#define KARR
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;

/* USER CODE BEGIN Variables */
#define AudioBUFFERLEN 128
static  int32_t audioBuffer[AudioBUFFERLEN];
#define MAX_BUF_SIZE 256
static char dataOut[MAX_BUF_SIZE];
TaskHandle_t micTask;
QueueHandle_t maQueue;

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */
void VmicTask(void* param);
void vLedsTimerCallback(TimerHandle_t* timer);
/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  ws2813_init_tim(&htim3, TIM_CHANNEL_1, 8);
  /* USER CODE END Init */

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
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  xTaskCreate(VmicTask,"MickTask",100,NULL,2,&micTask);

  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  maQueue = xQueueCreate(10,sizeof(int64_t));
  /* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Application */


void VmicTask(void* param){

	HAL_DFSDM_FilterRegularStart_DMA(&hdfsdm1_filter0,audioBuffer,AudioBUFFERLEN);
	while(1){
		int64_t maxVal;
		if(xQueueReceiveFromISR(maQueue,&maxVal,pdMS_TO_TICKS(10))==pdTRUE){

			WS2813_COLOR color_on={.rgb={.r=0,.g=0,.b=0}};
			color_on.rgb.r=63;
			for(int i=0;i<8;i++){
				ws2813_update_led(0, color_on);
			}

			snprintf( dataOut, MAX_BUF_SIZE, "\r\n la valeur max est : %d \r\n",(int)maxVal);

				HAL_UART_Transmit(&huart2,(uint8_t*)dataOut,strlen(dataOut),5000);

				if(maxVal < 10000){
					ws2813_update_led(0, color_on);
				}else if(maxVal>10000 && maxVal<20000){
					ws2813_update_led(1, color_on);
				}else if(maxVal>20000 && maxVal<30000){
					ws2813_update_led(2, color_on);
				}else if(maxVal>30000 && maxVal<40000){
					ws2813_update_led(3, color_on);
				}else if(maxVal>40000 && maxVal<50000){
					ws2813_update_led(4, color_on);
				}else if(maxVal>50000 && maxVal<60000){
					ws2813_update_led(5, color_on);
				}else if(maxVal>60000 && maxVal<70000){
					ws2813_update_led(6, color_on);
				}else if(maxVal>70000 && maxVal<80000){
					ws2813_update_led(7, color_on);
				}

				ws2813_start_transmission_tim();
		}
	}
}

void HAL_DFSDM_FilterRegConvCpltCallback(DFSDM_Filter_HandleTypeDef *hsfsdm_filter){

	uint64_t maxVal=0;
	int index=0;
	for(index=0;index<AudioBUFFERLEN;index++){
		if(maxVal < abs(audioBuffer[index])){
			maxVal = abs(audioBuffer[index]);
		}
	}

	xQueueSendFromISR(maQueue,&maxVal,NULL);

}



/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
