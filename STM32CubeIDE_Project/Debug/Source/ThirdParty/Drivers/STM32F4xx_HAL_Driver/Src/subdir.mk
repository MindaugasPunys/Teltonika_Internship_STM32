################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c \
../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.c \
../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc_ex.c \
../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_dma.c \
../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_exti.c \
../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_gpio.c \
../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rcc.c \
../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rtc.c \
../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_tim.c \
../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usart.c \
../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_utils.c 

OBJS += \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.o \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.o \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.o \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.o \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.o \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.o \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.o \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.o \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.o \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.o \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.o \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.o \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.o \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.o \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc_ex.o \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.o \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.o \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_dma.o \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_exti.o \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_gpio.o \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rcc.o \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rtc.o \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_tim.o \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usart.o \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_utils.o 

C_DEPS += \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.d \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.d \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.d \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.d \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.d \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.d \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.d \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.d \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.d \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.d \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.d \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.d \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.d \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.d \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc_ex.d \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.d \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.d \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_dma.d \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_exti.d \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_gpio.d \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rcc.d \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rtc.d \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_tim.d \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usart.d \
./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_utils.d 


# Each subdirectory must supply rules for building sources it contributes
Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/%.o Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/%.su: ../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/%.c Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DSTM32F413xx -DUSE_HAL_DRIVER -c -I../Source/ThirdParty/Core/Inc -I../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Inc -I../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/include -I../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Source/ThirdParty/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Source/ThirdParty/Drivers/CMSIS/Include -I../Source/APP/ -I../Source/API/ -I../Source/Driver/ -I../Source/Utility/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Source-2f-ThirdParty-2f-Drivers-2f-STM32F4xx_HAL_Driver-2f-Src

clean-Source-2f-ThirdParty-2f-Drivers-2f-STM32F4xx_HAL_Driver-2f-Src:
	-$(RM) ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.d ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.o ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.su ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.d ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.o ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.su ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.d ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.o ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.su ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.d ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.o ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.su ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.d ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.o ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.su ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.d ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.o ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.su ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.d ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.o ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.su ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.d ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.o ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.su ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.d ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.o ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.su ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.d ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.o ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.su ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.d ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.o ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.su ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.d ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.o ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.su ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.d ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.o ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.su ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.d ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.o ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.su ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc_ex.d ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc_ex.o ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc_ex.su ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.d ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.o ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.su ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.d ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.o ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.su ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_dma.d ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_dma.o ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_dma.su ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_exti.d ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_exti.o ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_exti.su ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_gpio.d ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_gpio.o ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_gpio.su ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rcc.d ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rcc.o ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rcc.su ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rtc.d ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rtc.o ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rtc.su ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_tim.d ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_tim.o ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_tim.su ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usart.d ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usart.o ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usart.su ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_utils.d ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_utils.o ./Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_utils.su

.PHONY: clean-Source-2f-ThirdParty-2f-Drivers-2f-STM32F4xx_HAL_Driver-2f-Src

