################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/Utility/circ_buffer.c \
../Source/Utility/message.c \
../Source/Utility/string_utils.c 

OBJS += \
./Source/Utility/circ_buffer.o \
./Source/Utility/message.o \
./Source/Utility/string_utils.o 

C_DEPS += \
./Source/Utility/circ_buffer.d \
./Source/Utility/message.d \
./Source/Utility/string_utils.d 


# Each subdirectory must supply rules for building sources it contributes
Source/Utility/%.o Source/Utility/%.su: ../Source/Utility/%.c Source/Utility/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DSTM32F413xx -DUSE_HAL_DRIVER -c -I../Source/ThirdParty/Core/Inc -I../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Inc -I../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/include -I../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Source/ThirdParty/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Source/ThirdParty/Drivers/CMSIS/Include -I../Source/APP/ -I../Source/API/ -I../Source/Driver/ -I../Source/Utility/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Source-2f-Utility

clean-Source-2f-Utility:
	-$(RM) ./Source/Utility/circ_buffer.d ./Source/Utility/circ_buffer.o ./Source/Utility/circ_buffer.su ./Source/Utility/message.d ./Source/Utility/message.o ./Source/Utility/message.su ./Source/Utility/string_utils.d ./Source/Utility/string_utils.o ./Source/Utility/string_utils.su

.PHONY: clean-Source-2f-Utility

