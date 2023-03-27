#ifndef __LED_APP__H__
#define __LED_APP__H__

/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "stdbool.h"
#include "stdint.h"
#include "led_api.h"
#include "cli_app.h"
/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/
typedef struct sLedAppTaskArgs_t {
    eCliAppCommandEnum_t task;
    void *task_args_struct;
} sLedAppTaskArgs_t;

/* Task argument structures */
typedef struct sLedAppTaskSet_t {
    eLedApiPinEnum_t pin;
} sLedAppTaskSet_t;

typedef struct sLedAppTaskReset_t {
    eLedApiPinEnum_t pin;
} sLedAppTaskReset_t;

typedef struct sLedAppTaskToggle_t {
    eLedApiPinEnum_t pin;
} sLedAppTaskToggle_t;

typedef struct sLedAppTaskBlink_t {
    eLedApiPinEnum_t pin;
    uint16_t time;
    uint16_t frequency;
} sLedAppTaskBlink_t;

/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
bool LED_APP_Init ();
bool LED_APP_AddTask (sLedAppTaskArgs_t *argument_struct);
#endif /* __LED_APP__H__ */
