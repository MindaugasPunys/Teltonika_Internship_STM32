/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "led_api.h"
#include "gpio_driver.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
typedef struct sLedApiStatic_t {
    eGpio_Enum_t gpio_pin;
    bool inverted;
} sLedApiStatic_t;
/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
static const sLedApiStatic_t static_led_api_lut[eLedApiPin_Last] = {
    [eLedApiPin_StatLed] = {.gpio_pin = eGpioDriver_StatLEDPin, .inverted = false},
    [eLedApiPin_GpsLed] = {.gpio_pin = eGpioDriver_GPSFixLEDPin, .inverted = true},
};
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
bool LED_API_Init (void) {
    for (eLedApiPinEnum_t led = eLedApiPin_First; led < eLedApiPin_Last; led++) {
        if (GPIO_Driver_Init(static_led_api_lut[led].gpio_pin) == false) {
            return false;
        }
        LED_API_ResetPin(led);
    }
    return true;
}

bool LED_API_SetPin (eLedApiPinEnum_t led) {
    if (led > eLedApiPin_Last) {
        return false;
    }

    if (static_led_api_lut[led].inverted == false) {
        return GPIO_Driver_SetPin(static_led_api_lut[led].gpio_pin);
    } else {
        return GPIO_Driver_ResetPin(static_led_api_lut[led].gpio_pin);
    }
}

bool LED_API_ResetPin (eLedApiPinEnum_t led) {
    if (led > eLedApiPin_Last) {
        return false;
    }

    if (static_led_api_lut[led].inverted == false) {
        return GPIO_Driver_ResetPin(static_led_api_lut[led].gpio_pin);
    } else {
        return GPIO_Driver_SetPin(static_led_api_lut[led].gpio_pin);
    }
}

bool LED_API_TogglePin (eLedApiPinEnum_t led) {
    if (led > eLedApiPin_Last) {
        return false;
    }
    return GPIO_Driver_TogglePin(static_led_api_lut[led].gpio_pin);
}

