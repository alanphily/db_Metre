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
#include "x_nucleo_iks01a2.h"
#include "x_nucleo_iks01a2_accelero.h"
#include "../LIBS/WS2813.h"
#include "usart.h"

//#define KARR
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;

/* USER CODE BEGIN Variables */
#define MAX_BUF_SIZE 256
static char dataOut[MAX_BUF_SIZE];
static void *LSM6DSL_X_0_handle = NULL;
/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */
void vAcquisitionCallback(TimerHandle_t timer);

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	BSP_ACCELERO_Init( LSM6DSL_X_0, &LSM6DSL_X_0_handle );
	BSP_ACCELERO_Sensor_Enable( LSM6DSL_X_0_handle );
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
  TimerHandle_t acquisitionTimer = xTimerCreate("Acquisition",
		  pdMS_TO_TICKS(100),
		  pdTRUE,
		  NULL,
		  vAcquisitionCallback);

  xTimerStart(acquisitionTimer, 10);
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
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

void vAcquisitionCallback(TimerHandle_t timer){
	uint8_t id;
	SensorAxes_t acceleration;
	uint8_t status;


	BSP_ACCELERO_Get_Instance( LSM6DSL_X_0_handle, &id );

	BSP_ACCELERO_IsInitialized( LSM6DSL_X_0_handle, &status );

	if ( status == 1 )
	{
		if ( BSP_ACCELERO_Get_Axes( LSM6DSL_X_0_handle, &acceleration ) == COMPONENT_ERROR )
		{
			acceleration.AXIS_X = 0;
			acceleration.AXIS_Y = 0;
			acceleration.AXIS_Z = 0;
		}

		WS2813_COLOR color_on={.rgb={.r=0,.g=0,.b=0}};


		for(int i=0;i<8;i++){
			ws2813_update_led(i,color_on);
		}


		if((int)acceleration.AXIS_X<-200){
			color_on.rgb.r=63;
			ws2813_update_led(0, color_on);
			snprintf( dataOut, MAX_BUF_SIZE,  "\r\n la led 1 s'allume \r\n");
		}
		else if((int)acceleration.AXIS_X>=-200 && (int)acceleration.AXIS_X<-100){
			color_on.rgb.r=63;
			ws2813_update_led(1, color_on);
			snprintf( dataOut, MAX_BUF_SIZE,  "\r\n la led 2 s'allume \r\n");
		}
		else if((int)acceleration.AXIS_X>=-100 && (int)acceleration.AXIS_X<-50){
			color_on.rgb.r=63;
			ws2813_update_led(2, color_on);
			snprintf( dataOut, MAX_BUF_SIZE,  "\r\n la led 3 s'allume \r\n");
		}
		else if((int)acceleration.AXIS_X>=-50 && (int)acceleration.AXIS_X<0){
			color_on.rgb.g=63;
			ws2813_update_led(3, color_on);
			snprintf( dataOut, MAX_BUF_SIZE,  "\r\n la led 4 s'allume \r\n");
		}
		else if((int)acceleration.AXIS_X>=0 && (int)acceleration.AXIS_X<50){
			color_on.rgb.g=63;
			ws2813_update_led(4, color_on);
			snprintf( dataOut, MAX_BUF_SIZE,  "\r\n la led 5 s'allume \r\n");
		}
		else if((int)acceleration.AXIS_X>=50 && (int)acceleration.AXIS_X<100){
			color_on.rgb.r=63;
			ws2813_update_led(5, color_on);
			snprintf( dataOut, MAX_BUF_SIZE,  "\r\n la led 6 s'allume \r\n");
		}
		else if((int)acceleration.AXIS_X>=100 && (int)acceleration.AXIS_X<200){
			color_on.rgb.r=63;
			ws2813_update_led(6, color_on);
			snprintf( dataOut, MAX_BUF_SIZE,  "\r\n la led 7 s'allume \r\n");
		}
		else if((int)acceleration.AXIS_X>200){
			color_on.rgb.r=63;
			ws2813_update_led(7, color_on);
			snprintf( dataOut, MAX_BUF_SIZE, "\r\n la led 8 s'allume \r\n");
		}

		// Transmet la trame aux LEDS
		HAL_UART_Transmit( &huart2, ( uint8_t * )dataOut, strlen( dataOut ), 5000 );

		ws2813_start_transmission_tim();
	}

}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
