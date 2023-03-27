/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "cmsis_os.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_usart.h"
#include "usart.h"
#include "stm32f4xx_ll_tim.h"
#include "timer_driver.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
void TIM_Timer13_init (void) {
    LL_TIM_InitTypeDef TIM_InitStruct = {0};

    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM13);

    NVIC_SetPriority(TIM8_UP_TIM13_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0));
    NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);

    TIM_InitStruct.Prescaler = (uint16_t)(960 - LL_TIM_IC_FILTER_FDIV1_N2);
    TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
    TIM_InitStruct.Autoreload = 65535;
    TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
    LL_TIM_Init(TIM13, &TIM_InitStruct);
    LL_TIM_DisableARRPreload(TIM13);
}

volatile unsigned long ulHighFrequencyTimerTicks;

void configureTimerForRunTimeStats (void) {
    ulHighFrequencyTimerTicks = 0;
    LL_TIM_EnableIT_UPDATE(TIM13);
    LL_TIM_EnableCounter(TIM13);
}

unsigned long getRunTimeCounterValue (void) {
    return ulHighFrequencyTimerTicks;
}

void TIM8_UP_TIM13_IRQHandler (void) {
    if (LL_TIM_IsActiveFlag_UPDATE(TIM13)) {
        ulHighFrequencyTimerTicks++;
    }
    LL_TIM_ClearFlag_UPDATE(TIM13);
}

