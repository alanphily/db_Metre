################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LIBS/WS2813.c 

OBJS += \
./LIBS/WS2813.o 

C_DEPS += \
./LIBS/WS2813.d 


# Each subdirectory must supply rules for building sources it contributes
LIBS/%.o: ../LIBS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32L476xx -I/media/sf_Code/stm/Projets/IMERIR/Drivers/WS2813/Inc -I../WS2813/LIBS -I/media/sf_Code/stm/Projets/IMERIR/Drivers/WS2813/Drivers/STM32L4xx_HAL_Driver/Inc -I/media/sf_Code/stm/Projets/IMERIR/Drivers/WS2813/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I/media/sf_Code/stm/Projets/IMERIR/Drivers/WS2813/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I/media/sf_Code/stm/Projets/IMERIR/Drivers/WS2813/Drivers/CMSIS/Device/ST/STM32L4xx/Include -I/media/sf_Code/stm/Projets/IMERIR/Drivers/WS2813/Middlewares/Third_Party/FreeRTOS/Source/include -I/media/sf_Code/stm/Projets/IMERIR/Drivers/WS2813/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I/media/sf_Code/stm/Projets/IMERIR/Drivers/WS2813/Drivers/CMSIS/Include -I"/home/imerir/Bureau/TempReel/STM32-WS2813-Driver-master-517b677a999572736d1c2774538eabe2e768a866/Inc" -I"/home/imerir/Bureau/TempReel/STM32-WS2813-Driver-master-517b677a999572736d1c2774538eabe2e768a866/Drivers/STM32L4xx_HAL_Driver/Inc" -I"/home/imerir/Bureau/TempReel/STM32-WS2813-Driver-master-517b677a999572736d1c2774538eabe2e768a866/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy" -I"/home/imerir/Bureau/TempReel/STM32-WS2813-Driver-master-517b677a999572736d1c2774538eabe2e768a866/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/home/imerir/Bureau/TempReel/STM32-WS2813-Driver-master-517b677a999572736d1c2774538eabe2e768a866/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"/home/imerir/Bureau/TempReel/STM32-WS2813-Driver-master-517b677a999572736d1c2774538eabe2e768a866/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/home/imerir/Bureau/TempReel/STM32-WS2813-Driver-master-517b677a999572736d1c2774538eabe2e768a866/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"/home/imerir/Bureau/TempReel/STM32-WS2813-Driver-master-517b677a999572736d1c2774538eabe2e768a866/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


