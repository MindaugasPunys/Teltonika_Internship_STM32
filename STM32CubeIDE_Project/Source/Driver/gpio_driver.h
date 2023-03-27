#ifndef __GPIO_DRIVER__H__
#define __GPIO_DRIVER__H__

/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "main.h"
#include "stdbool.h"

/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/
typedef enum eGpio_Enum_t {
    eGpioDriver_First = 0,
        eGpioDriver_ModemPowerOffPin = eGpioDriver_First,
        eGpioDriver_GPSFixLEDPin,
        eGpioDriver_StatLEDPin,
        eGpioDriver_ModemUARTDTRPin,
        eGpioDriver_ModemUARTRTSPin,
        eGpioDriver_ModemONPin,
        eGpioDriver_ModemUARTCTSPin,
        eGpioDriver_GNSSOnPin,
        eGpioDriver_UartUsart1TXPin,
        eGpioDriver_UartUsart1RXPin,
        eGpioDriver_UartUsart2TXPin,
        eGpioDriver_UartUsart2RXPin,
        eGpioDriver_Last
} eGpio_Enum_t;

/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
bool GPIO_Driver_Init (eGpio_Enum_t gpio);
bool GPIO_Driver_SetPin (eGpio_Enum_t gpio);
bool GPIO_Driver_ResetPin (eGpio_Enum_t gpio);
bool GPIO_Driver_TogglePin (eGpio_Enum_t gpio);
bool GPIO_Driver_ReadOutputPin (eGpio_Enum_t gpio, bool *read_value);
bool GPIO_Driver_ReadInputPin (eGpio_Enum_t gpio, bool *read_value);

#endif /* __GPIO_DRIVER__H__ */
