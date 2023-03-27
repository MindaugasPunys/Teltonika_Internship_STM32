/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "gpio_driver.h"

/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
typedef struct sGpioDriver_Desc_t {
    GPIO_TypeDef *port;
    uint32_t clock;
    uint32_t pin;
    uint32_t mode;
    uint32_t speed;
    uint32_t outputType;
    uint32_t pull;
    uint32_t alternate;
} sGpioDriver_Desc_t;

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
// ADD STATE TO INIT TO
// @formatter:off
const sGpioDriver_Desc_t static_gpio_lut[eGpioDriver_Last] = {
    [eGpioDriver_ModemPowerOffPin] = {.port = GPIOE, .clock = LL_AHB1_GRP1_PERIPH_GPIOE, .pin = Modem_Power_Off_Pin, .mode = LL_GPIO_MODE_OUTPUT,    .speed = LL_GPIO_SPEED_FREQ_LOW,       .outputType = LL_GPIO_OUTPUT_PUSHPULL, .pull = LL_GPIO_PULL_NO, .alternate = LL_GPIO_AF_0},
    [eGpioDriver_GPSFixLEDPin] =     {.port = GPIOB, .clock = LL_AHB1_GRP1_PERIPH_GPIOB, .pin = GPS_Fix_LED_Pin,     .mode = LL_GPIO_MODE_OUTPUT,    .speed = LL_GPIO_SPEED_FREQ_LOW,       .outputType = LL_GPIO_OUTPUT_PUSHPULL, .pull = LL_GPIO_PULL_NO, .alternate = LL_GPIO_AF_0},
    [eGpioDriver_StatLEDPin] =       {.port = GPIOB, .clock = LL_AHB1_GRP1_PERIPH_GPIOB, .pin = Stat_LED_Pin,        .mode = LL_GPIO_MODE_OUTPUT,    .speed = LL_GPIO_SPEED_FREQ_LOW,       .outputType = LL_GPIO_OUTPUT_PUSHPULL, .pull = LL_GPIO_PULL_NO, .alternate = LL_GPIO_AF_0},
    [eGpioDriver_ModemUARTDTRPin] =  {.port = GPIOD, .clock = LL_AHB1_GRP1_PERIPH_GPIOD, .pin = Modem_UART_DTR_Pin,  .mode = LL_GPIO_MODE_OUTPUT,    .speed = LL_GPIO_SPEED_FREQ_LOW,       .outputType = LL_GPIO_OUTPUT_PUSHPULL, .pull = LL_GPIO_PULL_NO, .alternate = LL_GPIO_AF_0},
    [eGpioDriver_ModemUARTRTSPin] =  {.port = GPIOD, .clock = LL_AHB1_GRP1_PERIPH_GPIOD, .pin = Modem_UART_RTS_Pin,  .mode = LL_GPIO_MODE_OUTPUT,    .speed = LL_GPIO_SPEED_FREQ_LOW,       .outputType = LL_GPIO_OUTPUT_PUSHPULL, .pull = LL_GPIO_PULL_NO, .alternate = LL_GPIO_AF_0},
    [eGpioDriver_ModemONPin] =       {.port = GPIOA, .clock = LL_AHB1_GRP1_PERIPH_GPIOA, .pin = Modem_ON_Pin,        .mode = LL_GPIO_MODE_OUTPUT,    .speed = LL_GPIO_SPEED_FREQ_LOW,       .outputType = LL_GPIO_OUTPUT_PUSHPULL, .pull = LL_GPIO_PULL_NO, .alternate = LL_GPIO_AF_0},
    [eGpioDriver_ModemUARTCTSPin] =  {.port = GPIOD, .clock = LL_AHB1_GRP1_PERIPH_GPIOD, .pin = Modem_UART_CTS_Pin,  .mode = LL_GPIO_MODE_INPUT,     .speed = LL_GPIO_SPEED_FREQ_LOW,       .outputType = LL_GPIO_OUTPUT_PUSHPULL, .pull = LL_GPIO_PULL_NO, .alternate = LL_GPIO_AF_0},
    [eGpioDriver_GNSSOnPin] =        {.port = GPIOG, .clock = LL_AHB1_GRP1_PERIPH_GPIOG, .pin = GNSS_On_Pin,         .mode = LL_GPIO_MODE_OUTPUT,    .speed = LL_GPIO_SPEED_FREQ_LOW,       .outputType = LL_GPIO_OUTPUT_PUSHPULL, .pull = LL_GPIO_PULL_NO, .alternate = LL_GPIO_AF_0},
    [eGpioDriver_UartUsart1TXPin] =  {.port = GPIOB, .clock = LL_AHB1_GRP1_PERIPH_GPIOB, .pin = Debug_TX_Pin,        .mode = LL_GPIO_MODE_ALTERNATE, .speed = LL_GPIO_SPEED_FREQ_VERY_HIGH, .outputType = LL_GPIO_OUTPUT_PUSHPULL, .pull = LL_GPIO_PULL_NO, .alternate = LL_GPIO_AF_7},
    [eGpioDriver_UartUsart1RXPin] =  {.port = GPIOB, .clock = LL_AHB1_GRP1_PERIPH_GPIOB, .pin = Debug_RX_Pin,        .mode = LL_GPIO_MODE_ALTERNATE, .speed = LL_GPIO_SPEED_FREQ_VERY_HIGH, .outputType = LL_GPIO_OUTPUT_PUSHPULL, .pull = LL_GPIO_PULL_NO, .alternate = LL_GPIO_AF_7},
    [eGpioDriver_UartUsart2TXPin] =  {.port = GPIOD, .clock = LL_AHB1_GRP1_PERIPH_GPIOD, .pin = Modem_UART_TX_Pin,   .mode = LL_GPIO_MODE_ALTERNATE, .speed = LL_GPIO_SPEED_FREQ_VERY_HIGH, .outputType = LL_GPIO_OUTPUT_PUSHPULL, .pull = LL_GPIO_PULL_NO, .alternate = LL_GPIO_AF_7},
    [eGpioDriver_UartUsart2RXPin] =  {.port = GPIOD, .clock = LL_AHB1_GRP1_PERIPH_GPIOD, .pin = Modem_UART_RX_Pin,   .mode = LL_GPIO_MODE_ALTERNATE, .speed = LL_GPIO_SPEED_FREQ_VERY_HIGH, .outputType = LL_GPIO_OUTPUT_PUSHPULL, .pull = LL_GPIO_PULL_NO, .alternate = LL_GPIO_AF_7}
};
// @formatter:on
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
bool GPIO_Driver_Init (eGpio_Enum_t gpio) {

    if (gpio >= eGpioDriver_Last) {
        return false;
    }

    /* GPIO Ports Clock Enable */
    LL_AHB1_GRP1_EnableClock(static_gpio_lut[gpio].clock);
    LL_GPIO_ResetOutputPin(static_gpio_lut[gpio].port, static_gpio_lut[gpio].pin);

    /* Init GPIO struct */
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = static_gpio_lut[gpio].pin;
    GPIO_InitStruct.Mode = static_gpio_lut[gpio].mode;
    GPIO_InitStruct.Speed = static_gpio_lut[gpio].speed;
    GPIO_InitStruct.OutputType = static_gpio_lut[gpio].outputType;
    GPIO_InitStruct.Pull = static_gpio_lut[gpio].pull;
    GPIO_InitStruct.Alternate = static_gpio_lut[gpio].alternate;

    ErrorStatus status = ERROR;
    status = LL_GPIO_Init(static_gpio_lut[gpio].port, &GPIO_InitStruct);
    if (status != SUCCESS) {
        return false;
    }
    return true;
}

bool GPIO_Driver_SetPin (eGpio_Enum_t gpio) {
    if (gpio >= eGpioDriver_Last) {
        return false;
    }

    LL_GPIO_SetOutputPin(static_gpio_lut[gpio].port, static_gpio_lut[gpio].pin);
    return true;
}

bool GPIO_Driver_ResetPin (eGpio_Enum_t gpio) {
    if (gpio >= eGpioDriver_Last) {
        return false;
    }

    LL_GPIO_ResetOutputPin(static_gpio_lut[gpio].port, static_gpio_lut[gpio].pin);
    return true;
}

bool GPIO_Driver_TogglePin (eGpio_Enum_t gpio) {
    if (gpio >= eGpioDriver_Last) {
        return false;
    }

    LL_GPIO_TogglePin(static_gpio_lut[gpio].port, static_gpio_lut[gpio].pin);
    return true;
}

bool GPIO_Driver_ReadOutputPin (eGpio_Enum_t gpio, bool *read_value) {
    if (gpio >= eGpioDriver_Last) {
        return false;
    }

    *read_value = (bool) LL_GPIO_ReadOutputPort(static_gpio_lut[gpio].port);
    return true;
}

bool GPIO_Driver_ReadInputPin (eGpio_Enum_t gpio, bool *read_value) {
    if (gpio >= eGpioDriver_Last) {
        return false;
    }

    *read_value = (bool) LL_GPIO_ReadInputPort(static_gpio_lut[gpio].port);
    return true;
}
