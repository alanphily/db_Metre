/* 
 *   STM32 WS2813 Driver name and a brief idea of what it does.>
 *   Copyright (C) 2017 Aurelien Valade
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *
 * WS2813.c
 *
 * This module is a hardware driver to emulate WS2813 signals through STM32
 * hardware SPI at 2.5MHz using DMAs to speed-up the process.
 *
 * This library is designed to be used with FreeRTOS.
 *
 *  Created on: 27 oct. 2017
 *      Author: Aurelien VALADE
 */
#include "../LIBS/WS2813.h"

#include <stdlib.h>
#include <string.h>

#include <FreeRTOS.h>
#include <semphr.h>

/* Private variables */
static int __leds_count;
static WS2813_COLOR *__led_colors;

static TIM_HandleTypeDef *__htim;
static uint32_t __tim_channel;
static uint16_t* __tim_data=NULL;

static SemaphoreHandle_t __DMA_busy;
static SemaphoreHandle_t __Transmission_busy;

#define TIM_HIGH (30)
#define TIM_LOW (10)

/* Public functions */


/**
 * @brief Initialize the WS2813 module
 *
 * At this point, the TIM module should already be configured for a period of 1.25ms
 *
 * @param htim the TIM module handler to use for transmission
 * @param leds_count the number of LEDs signals to transmit per frame
 */

void ws2813_init_tim(TIM_HandleTypeDef *htim, uint32_t channel, int leds_count)
{
	// Copy the information into local buffers
	__htim = htim;
	__leds_count = leds_count;
	__tim_channel = channel;

	// Ensure no previous Initialization has been done
	if (__tim_data != NULL)
	{
		free(__tim_data);
		free(__led_colors);
	}

	// Allocate the LEDs colors
	__led_colors = (WS2813_COLOR*)malloc(leds_count*sizeof(WS2813_COLOR));
	memset(__led_colors, 0, (leds_count)*sizeof(WS2813_COLOR));

	// Allocate the data buffer for maximum possible data count
	// The maximum bit length is 3 bits. This, the maximum frame length is 9*leds_count bits
	// Or 3*leds_count bytes
	__tim_data = (uint32_t*)malloc((leds_count*3*8+1)*sizeof(uint16_t));
	memset(__tim_data, 0, (leds_count*3*8+1)*sizeof(uint16_t));

	// Create the semaphore
	__DMA_busy = xSemaphoreCreateBinary();

	// Create the transmission busy semaphore and set it free
	__Transmission_busy = xSemaphoreCreateBinary();
	xSemaphoreGive(__Transmission_busy);
}


/**
 * @brief Update a LED color with a new value
 *
 * @param led_num The LED number (0..led_count-1)
 * @param color The new color to assign to the LED
 */
void ws2813_update_led(int led_num, WS2813_COLOR color)
{
	// If the module is initialized (so memory is reserved
	if (led_num < __leds_count && __tim_data != NULL)
		// Assign the color to the corresponding LED
		__led_colors[led_num] = color;
}


/**
 * @brief Start the data transmission
 *
 * Generate the Frame to be transmitted and start the transmission
 */
void ws2813_start_transmission_tim()
{
	int32_t bit_index = 0;
	int i,j,col;

	// Try to take the semaphore for data transmission
	if (xSemaphoreTake(__Transmission_busy, pdMS_TO_TICKS(10)) != pdTRUE)
		return;

	// For each LED
	for (i=0;i<__leds_count;i++)
	{
		// For each color
		for (col=0;col<3;col++)
		{
			// For each bit
			for (j=7;j>=0;j--)
			{
				if (((__led_colors[i].bytes[col]) & (1<<j)) == 0)
				{
					// Transmit a 0
					__tim_data[bit_index] = TIM_LOW;
				}
				else
				{
					// Transmit a 1
					__tim_data[bit_index] = TIM_HIGH;
				}
				bit_index++;
			}
		}
	}
	// Trim the data with a 0 to avoid unwanted glitch at startup
	__tim_data[bit_index++]=0;

	// Launch the transmission of data through DMA
	HAL_TIM_PWM_Start_DMA(__htim, __tim_channel, __tim_data, (__leds_count*3*8)+1);
}

/**
 * @brief Timer PWM Pulse Finished Callback
 *
 * This function is automatically called at the end of the data transmission through DMA
 * transfer on a timer PWM output
 */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == __htim)
	{
		// Frees the semaphore
		xSemaphoreGiveFromISR(__Transmission_busy, NULL);
		// And stop the PWM
		HAL_TIM_PWM_Stop_DMA(htim, __tim_channel);
	}
}
