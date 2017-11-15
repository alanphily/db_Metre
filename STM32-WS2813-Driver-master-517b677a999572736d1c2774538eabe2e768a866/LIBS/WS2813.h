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
 * WS2813.h
 *
 * This module is a hardware driver to emulate WS2813 signals through STM32
 * hardware SPI at 2.5MHz using DMAs to speed-up the process.
 *
 * This library is designed to be used with FreeRTOS.
 *
 *  Created on: 27 oct. 2017
 *      Author: Aurelien VALADE
 */

#ifndef WS2813_H_
#define WS2813_H_

#include <stdint.h>
//#include "spi.h"
#include "tim.h"

typedef union {
	struct{
	uint8_t g;
	uint8_t r;
	uint8_t b;
	} rgb;
	uint8_t bytes[3];
}WS2813_COLOR;


void ws2813_init_tim(TIM_HandleTypeDef *htim, uint32_t timer_channel, int leds_count);
void ws2813_update_led(int led_num, WS2813_COLOR color);
void ws2813_start_transmission_tim();


#endif /* WS2813_H_ */
