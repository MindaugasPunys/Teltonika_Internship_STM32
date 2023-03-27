/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <network_app.h>
#include "cmd_api_callbacks.h"

#include "string_utils.h"
#include "cli_app.h"
#include "led_app.h"
//#include "gpio_driver.h"
#include "heap_api.h"
#include "debug_api.h"
#include "uart_api.h"
#include "modem_api.h"
#include "stddef.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "cmsis_os2.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define BAD_CHARS " "
#define DELIM ","
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
static bool CMD_API_ParsePin (sCmdApi_CallbackArgs_t *cmd_api_callback_args, eLedApiPinEnum_t *pin) {
    /* Assert */
    if (cmd_api_callback_args->arguments == NULL) {
        return false;
    }
    StrRemChar(cmd_api_callback_args->arguments, BAD_CHARS);
    bool status = false;

    /* Parse LED */
    char *token = strtok(cmd_api_callback_args->arguments, DELIM);
    status = StrToUint(token, (uint16_t*) pin);
    if (status != true) {
        snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
        "Argument LED-\"%s\" bad", token);
        return false;
    }
    return true;
}

/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool CMD_API_SetCallback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    eLedApiPinEnum_t pin = eLedApiPin_Last;
    if (CMD_API_ParsePin(cmd_api_callback_args, &pin) == false) {
        return false;
    }

    /* Add arguments to message queue */
    sLedAppTaskSet_t *set_task_arguments = Heap_API_Allocate(1, sizeof(sLedAppTaskSet_t));
    set_task_arguments->pin = pin;
    sLedAppTaskArgs_t task_args_message = {
        .task = eCliAppCmd_SetLed,
        .task_args_struct = set_task_arguments
    };
    if (LED_APP_AddTask(&task_args_message) == false) {
        snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
        static_debug_message_lut[eDebugMsg_MessageQueuePutFailed]);
        return false;
    }

    /* Creating message buffer */
    snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
    "Set: Pin-%u", pin);
    return true;
}
bool CMD_API_ResetCallback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    eLedApiPinEnum_t pin = eLedApiPin_Last;
    if (CMD_API_ParsePin(cmd_api_callback_args, &pin) == false) {
        return false;
    }

    /* Add arguments to message queue */
    sLedAppTaskReset_t *reset_task_arguments = Heap_API_Allocate(1, sizeof(sLedAppTaskReset_t));
    reset_task_arguments->pin = pin;
    sLedAppTaskArgs_t task_args_message = {
        .task = eCliAppCmd_ResetLed,
        .task_args_struct = reset_task_arguments
    };
    if (LED_APP_AddTask(&task_args_message) == false) {
        snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
        static_debug_message_lut[eDebugMsg_MessageQueuePutFailed]);
        return false;
    }

    /* Creating message buffer */
    snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
    "Reset: Pin-%u", pin);
    return true;
}
bool CMD_API_ToggleCallback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    eLedApiPinEnum_t pin = eLedApiPin_Last;
    if (CMD_API_ParsePin(cmd_api_callback_args, &pin) == false) {
        return false;
    }

    /* Add arguments to message queue */
    sLedAppTaskToggle_t *toggle_task_arguments = Heap_API_Allocate(1, sizeof(sLedAppTaskToggle_t));
    toggle_task_arguments->pin = pin;
    sLedAppTaskArgs_t task_args_message = {
        .task = eCliAppCmd_ToggleLed,
        .task_args_struct = toggle_task_arguments
    };
    if (LED_APP_AddTask(&task_args_message) == false) {
        snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
        static_debug_message_lut[eDebugMsg_MessageQueuePutFailed]);
        return false;
    }

    /* Creating message buffer */
    snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
    "Toggle: Pin-%u", pin);
    return true;
}
bool CMD_API_BlinkCallback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    /* Assert */
    eLedApiPinEnum_t pin = eLedApiPin_Last;
    if (CMD_API_ParsePin(cmd_api_callback_args, &pin) == false) {
        return false;
    }

    uint16_t time = 0;
    char *token = strtok(NULL, ",");
    bool status = StrToUint(token, &time);
    if (status != true) {
        snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
        "Argument Time-\"%s\" bad", token);
        return false;
    }

    uint16_t freq = 0;
    token = strtok(NULL, ",");
    status = StrToUint(token, &freq);
    if (status != true) {
        snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
        "Argument Frequency-\"%s\" bad", token);
        return false;
    }

    /* Add arguments to message queue */
    sLedAppTaskBlink_t *blink_task_arguments = Heap_API_Allocate(1, sizeof(sLedAppTaskBlink_t));
    blink_task_arguments->pin = pin;
    blink_task_arguments->time = time;
    blink_task_arguments->frequency = freq;
    sLedAppTaskArgs_t task_args_message = {
        .task = eCliAppCmd_BlinkLed,
        .task_args_struct = blink_task_arguments
    };
    if (LED_APP_AddTask(&task_args_message) == false) {
        snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
        static_debug_message_lut[eDebugMsg_MessageQueuePutFailed]);
        return false;
    }

    /* Creating message buffer */
    snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
    "Blink: Pin-%u | %u sec | %u Hz", pin, time, freq);
    return true;
}

bool CMD_API_ModemCallback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    /* Assert */
    if (cmd_api_callback_args->arguments == NULL) {
        return false;
    }

//    strcat(cmd_api_callback_args->arguments, "\n");
    Modem_API_Send(cmd_api_callback_args->arguments);

    /* Creating message buffer */
    strncpy(cmd_api_callback_args->response_buffer, "", cmd_api_callback_args->response_buffer_length);
    return true;
}

bool CMD_API_ModemStartTcpCallback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    /* Assert */
    if (cmd_api_callback_args->arguments == NULL) {
        return false;
    }

    char *ip = strtok(cmd_api_callback_args->arguments, ",");

    uint16_t port = 0;
    char *token = strtok(NULL, ",");
    StrToUint(token, &port);

    char *message = strtok(NULL, ",");
    Network_APP_SetState(true, ip, port, message);

    /* Creating message buffer */
    strncpy(cmd_api_callback_args->response_buffer, "", cmd_api_callback_args->response_buffer_length);
    return true;
}
bool CMD_API_ModemStopTcpCallback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    /* Assert */
    Network_APP_SetState(false, NULL, 0, NULL);

    /* Creating message buffer */
    strncpy(cmd_api_callback_args->response_buffer, "", cmd_api_callback_args->response_buffer_length);
    return true;
}
