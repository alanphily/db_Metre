# STM32 Driver for WS2813

This project is a demonstration code for _STM32L476RG_ Nucleo development board using WS2813 addressable LEDs strip.

The project is designed to work with FreeRTOS, using system TIMER and DMA to generate proper frames. The peripherals have to be initialized BEFORE using the library.

## Example code

The example project has beed created using CubeMX to configure the TIMER as follows:
- The timer generates a fixed frame lenght of 1.25ms,
- The timer channel is configured as PWM Mode 1,
- The corresponding GPIO is set to high-speed,
- The DMA is configured to operate on the selected timer channel.

The code is set to run a Knight Rider example on a 8 LEDs strip.

## Code organisation

The main library code is set into the **LIB** folder. The rest of the code is into the **Src/freertos.c** file.  

## Adapting the code

To adapt the code to your project, take care about:
- Checking your timer speed,
- Adapt the TIM\_HIGH and TIM\_LOW values from file LIBS/WS2813.c accordingly to get 800ns (resp 400ns) delays,
- Adapt the configuration initialization to your needs.

## License

This code is Licensed under GPLv3.

    STM32 WS2813 Driver for FreeRTOS using Timers and DMA
	Copyright (C) 2017 Aurelien Valade

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>