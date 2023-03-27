################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/APP/cli_app.c \
../Source/APP/led_app.c \
../Source/APP/main.c \
../Source/APP/network_app.c \
../Source/APP/sms_app.c 

OBJS += \
./Source/APP/cli_app.o \
./Source/APP/led_app.o \
./Source/APP/main.o \
./Source/APP/network_app.o \
./Source/APP/sms_app.o 

C_DEPS += \
./Source/APP/cli_app.d \
./Source/APP/led_app.d \
./Source/APP/main.d \
./Source/APP/network_app.d \
./Source/APP/sms_app.d 


# Each subdirectory must supply rules for building sources it contributes
Source/APP/%.o Source/APP/%.su: ../Source/APP/%.c Source/APP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DSTM32F413xx -DUSE_HAL_DRIVER -c -I../Source/ThirdParty/Core/Inc -I../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Inc -I../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/include -I../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Source/ThirdParty/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Source/ThirdParty/Drivers/CMSIS/Include -I../Source/APP/ -I../Source/API/ -I../Source/Driver/ -I../Source/Utility/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Source-2f-APP

clean-Source-2f-APP:
	-$(RM) ./Source/APP/cli_app.d ./Source/APP/cli_app.o ./Source/APP/cli_app.su ./Source/APP/led_app.d ./Source/APP/led_app.o ./Source/APP/led_app.su ./Source/APP/main.d ./Source/APP/main.o ./Source/APP/main.su ./Source/APP/network_app.d ./Source/APP/network_app.o ./Source/APP/network_app.su ./Source/APP/sms_app.d ./Source/APP/sms_app.o ./Source/APP/sms_app.su

.PHONY: clean-Source-2f-APP

