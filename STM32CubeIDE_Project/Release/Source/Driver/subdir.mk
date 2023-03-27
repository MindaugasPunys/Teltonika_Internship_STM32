################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/Driver/timer_driver.c \
../Source/Driver/uart_driver.c 

OBJS += \
./Source/Driver/timer_driver.o \
./Source/Driver/uart_driver.o 

C_DEPS += \
./Source/Driver/timer_driver.d \
./Source/Driver/uart_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Source/Driver/%.o Source/Driver/%.su: ../Source/Driver/%.c Source/Driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_FULL_LL_DRIVER -DSTM32F413xx -DUSE_HAL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Source-2f-Driver

clean-Source-2f-Driver:
	-$(RM) ./Source/Driver/timer_driver.d ./Source/Driver/timer_driver.o ./Source/Driver/timer_driver.su ./Source/Driver/uart_driver.d ./Source/Driver/uart_driver.o ./Source/Driver/uart_driver.su

.PHONY: clean-Source-2f-Driver

