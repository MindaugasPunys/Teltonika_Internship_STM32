/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <network_app.h>
#include "cmsis_os.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_usart.h"
#include "usart.h"
#include "stm32f4xx_ll_tim.h"
#include "stdbool.h"
#include "string.h"
#include "stdlib.h"

#include "timer_driver.h"
#include "gpio_driver.h"
#include "circ_buffer.h"
#include "uart_api.h"
#include "debug_api.h"
#include "heap_api.h"
#include "cli_app.h"
#include "led_app.h"
#include "modem_api.h"
#include "sms_app.h"

#include "rtc_driver.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
//INIT_DEBUG_MODULE(MAIN)
/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/
static void SystemClock_Config (void);
/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
static void SystemClock_Config (void) {
    /* LATENCY */
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);
    while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_3) {
    }
    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
    LL_RCC_HSI_SetCalibTrimming(16);
    LL_RCC_HSI_Enable();

    /* LSI */
    LL_RCC_LSI_Enable();
    while (LL_RCC_LSI_IsReady() != 1) {
    }
    LL_PWR_EnableBkUpAccess();

    /* PLL CONFIG */
    while (LL_RCC_HSI_IsReady() != 1) {
    }
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_8, 100, LL_RCC_PLLP_DIV_2);
    LL_RCC_PLL_Enable();
    while (LL_RCC_PLL_IsReady() != 1) {
    }
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {
    }
    LL_SetSystemCoreClock(100000000);
    if (HAL_InitTick(TICK_INT_PRIORITY) != HAL_OK) {
        __disable_irq();
        while (1) {
        }
    }
    LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);
}

/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/

int main (void) {
    HAL_Init();
    SystemClock_Config();
    osKernelInitialize();

    TIM_Timer13_init(); /* Timer for RTos task handling */

    RTC_Driver_Init();

    /* API AND APIS */
    Heap_API_Init();
    Debug_API_Init();

    Modem_API_Init();
    SMS_APP_Init();
    Network_APP_Init();

    LED_APP_Init();
    CLI_APP_Init();

    osKernelStart();
    while (true) {
    }
}

void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        HAL_IncTick();
    }
}
// ________________________________________________________________________________________________________________________________________
