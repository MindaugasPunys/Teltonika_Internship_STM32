################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/croutine.c \
../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c \
../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/list.c \
../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/queue.c \
../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c \
../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/tasks.c \
../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/timers.c 

OBJS += \
./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/croutine.o \
./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/event_groups.o \
./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/list.o \
./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/queue.o \
./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.o \
./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/tasks.o \
./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/timers.o 

C_DEPS += \
./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/croutine.d \
./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/event_groups.d \
./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/list.d \
./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/queue.d \
./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.d \
./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/tasks.d \
./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/timers.d 


# Each subdirectory must supply rules for building sources it contributes
Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/%.o Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/%.su: ../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/%.c Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_FULL_LL_DRIVER -DSTM32F413xx -DUSE_HAL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Source-2f-ThirdParty-2f-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-Source

clean-Source-2f-ThirdParty-2f-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-Source:
	-$(RM) ./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/croutine.d ./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/croutine.o ./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/croutine.su ./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/event_groups.d ./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/event_groups.o ./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/event_groups.su ./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/list.d ./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/list.o ./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/list.su ./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/queue.d ./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/queue.o ./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/queue.su ./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.d ./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.o ./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.su ./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/tasks.d ./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/tasks.o ./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/tasks.su ./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/timers.d ./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/timers.o ./Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/timers.su

.PHONY: clean-Source-2f-ThirdParty-2f-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-Source

