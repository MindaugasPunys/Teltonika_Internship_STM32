/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "cli_app.h"
#include "cmd_api.h"

#include "debug_api.h"
#include "message.h"
#include "uart_api.h"
#include "heap_api.h"

#include "cmsis_os2.h"
#include "string.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define RESPONSE_BUFFER_LENGTH 128
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
#define SEPARATOR ":"
/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
INIT_DEBUG_MODULE(CLI_APP);
static const osThreadAttr_t cli_app_task_attributes = {.name = "CLI APP task", .priority = (osPriority_t) osPriorityNormal, .stack_size = 1024};

// @formatter:off
const sCmdApiStaticLut_t static_cli_app_lut[eCliAppCmd_Last] = {
    [eCliAppCmd_SetLed]         = {.cmd_name = "set",       .cmd_callback = CMD_API_SetCallback},
    [eCliAppCmd_ResetLed]       = {.cmd_name = "reset",     .cmd_callback = CMD_API_ResetCallback},
    [eCliAppCmd_ToggleLed]      = {.cmd_name = "toggle",    .cmd_callback = CMD_API_ToggleCallback},
    [eCliAppCmd_BlinkLed]       = {.cmd_name = "blink",     .cmd_callback = CMD_API_BlinkCallback},
    [eCliAppCmd_ModemCmd]       = {.cmd_name = "modem",     .cmd_callback = CMD_API_ModemCallback},
    [eCliAppCmd_ModemStartTcp]  = {.cmd_name = "start",     .cmd_callback = CMD_API_ModemStartTcpCallback},
    [eCliAppCmd_ModemStopTcp]   = {.cmd_name = "stop",      .cmd_callback = CMD_API_ModemStopTcpCallback},
};
// @formatter:on0

/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
static osThreadId_t cli_app_task_handle;
static sMessage_t uart_debug_message = {0};
static sCmdApiParserArgs_t cmd_api_args = {0};
static char cli_app_response_buffer[RESPONSE_BUFFER_LENGTH] = {0};
/**********************************************************************************************************************
 * Exported variables and references
 ************************************************************W*********************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/
static void CLI_APP_Thread (void *argument);
/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
static void CLI_APP_Thread (void *argument) {
    while (true) {
        if (UART_API_GetMessage(&uart_debug_message, eUart_Debug) == true) {

            cmd_api_args.message = uart_debug_message.buffer;
            cmd_api_args.message_length = uart_debug_message.length;
            cmd_api_args.response_buffer = cli_app_response_buffer;
            cmd_api_args.response_buffer_length = RESPONSE_BUFFER_LENGTH;
            cmd_api_args.static_lut_length = (uint16_t) eCliAppCmd_Last;
            cmd_api_args.static_lut = (sCmdApiStaticLut_t*) &static_cli_app_lut;

            CMD_API_ParseCommand(&cmd_api_args, SEPARATOR);

            if (strcmp(cmd_api_args.response_buffer, "") != 0) {
                debug("%s", cmd_api_args.response_buffer);
            }
            Heap_API_Free(uart_debug_message.buffer);
        }
    }
    osThreadTerminate(NULL);
}

/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool CLI_APP_Init (void) {
    /* Create new Thread */
    cli_app_task_handle = osThreadNew(CLI_APP_Thread, NULL, &cli_app_task_attributes);
    if (cli_app_task_handle == NULL) {
        error("Failed to initialize CLI APP task\n");
        return false;
    }
    return true;
}
