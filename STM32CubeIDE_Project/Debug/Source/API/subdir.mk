################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/API/cmd_api.c \
../Source/API/cmd_api_callbacks.c \
../Source/API/debug_api.c \
../Source/API/heap_api.c \
../Source/API/led_api.c \
../Source/API/modem_api.c \
../Source/API/modem_api_callbacks.c \
../Source/API/ntp_api.c \
../Source/API/uart_api.c 

OBJS += \
./Source/API/cmd_api.o \
./Source/API/cmd_api_callbacks.o \
./Source/API/debug_api.o \
./Source/API/heap_api.o \
./Source/API/led_api.o \
./Source/API/modem_api.o \
./Source/API/modem_api_callbacks.o \
./Source/API/ntp_api.o \
./Source/API/uart_api.o 

C_DEPS += \
./Source/API/cmd_api.d \
./Source/API/cmd_api_callbacks.d \
./Source/API/debug_api.d \
./Source/API/heap_api.d \
./Source/API/led_api.d \
./Source/API/modem_api.d \
./Source/API/modem_api_callbacks.d \
./Source/API/ntp_api.d \
./Source/API/uart_api.d 


# Each subdirectory must supply rules for building sources it contributes
Source/API/%.o Source/API/%.su: ../Source/API/%.c Source/API/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DSTM32F413xx -DUSE_HAL_DRIVER -c -I../Source/ThirdParty/Core/Inc -I../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Inc -I../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/include -I../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Source/ThirdParty/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Source/ThirdParty/Drivers/CMSIS/Include -I../Source/APP/ -I../Source/API/ -I../Source/Driver/ -I../Source/Utility/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Source-2f-API

clean-Source-2f-API:
	-$(RM) ./Source/API/cmd_api.d ./Source/API/cmd_api.o ./Source/API/cmd_api.su ./Source/API/cmd_api_callbacks.d ./Source/API/cmd_api_callbacks.o ./Source/API/cmd_api_callbacks.su ./Source/API/debug_api.d ./Source/API/debug_api.o ./Source/API/debug_api.su ./Source/API/heap_api.d ./Source/API/heap_api.o ./Source/API/heap_api.su ./Source/API/led_api.d ./Source/API/led_api.o ./Source/API/led_api.su ./Source/API/modem_api.d ./Source/API/modem_api.o ./Source/API/modem_api.su ./Source/API/modem_api_callbacks.d ./Source/API/modem_api_callbacks.o ./Source/API/modem_api_callbacks.su ./Source/API/ntp_api.d ./Source/API/ntp_api.o ./Source/API/ntp_api.su ./Source/API/uart_api.d ./Source/API/uart_api.o ./Source/API/uart_api.su

.PHONY: clean-Source-2f-API

