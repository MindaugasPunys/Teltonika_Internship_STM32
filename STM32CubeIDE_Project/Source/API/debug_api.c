/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "debug_api.h"
#include "uart_api.h"
#include "string_utils.h"

#include "stdarg.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define MESSAGE_LENGTH 1024
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
// ENUM FOR DEBUG
/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
static const osMutexAttr_t mutex_attributes = {
    "debug_api_mutex", // human readable MUTEX name
    0, // attr_bits
    NULL, // memory for control block
    0U // size for control block
};

/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
static char message[MESSAGE_LENGTH] = {0};
static osMutexId_t debug_mutex_id;
/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/
// Separate into parts
const char *static_debug_message_lut[eDebugMsg_Last] = {
    [eDebugMsg_ArgumentBad] = "Argument is bad!",
    [eDebugMsg_ArgumentBadSpecify] = "Argument bad: ",
    [eDebugMsg_MessageQueuePutFailed] = "Failed to add args to message queue!",
    [eDebugMsg_MessageQueueGetFailed] = "Failed to get args from message queue!",
    [eDebugMsg_ArgumentEmpty] = "Argument empty!",
    [eDebugMsg_ArgumentNotFound] = "Argument not found!",
};

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool Debug_API_Init () {
    debug_mutex_id = osMutexNew(&mutex_attributes);
    if (debug_mutex_id == NULL) {
        return false;
    }
    UART_API_Init(eUart_Debug, "\r\n", eUartBaudrate_921600);
    return true;
}

// swap buffer with message
bool Debug_API_Print (const char *module, const char *file_path, const int line, const char *format, ...) {
    if (osMutexAcquire(debug_mutex_id, osWaitForever) != osOK) {
        return false;
    }
    bool status = false;
    uint16_t index = 0;

    if (strcmp(module, MODEM_MODULE) == 0) {
        index = snprintf(message, MESSAGE_LENGTH, "[MODEM]\t");
    } else if (file_path == 0) {
        index = snprintf(message, MESSAGE_LENGTH, "[%s]\t[DEBUG]\t", module);
    } else {
        index = snprintf(message, MESSAGE_LENGTH, "[%s]\t[ERROR]\t(%s (%d))\t", module, file_path, line);
    }

    /* VA_ARGS */
    va_list args;
    va_start(args, format);
    index += vsnprintf(message + index, MESSAGE_LENGTH - index, format, args);
    va_end(args);

    status = UART_API_SendString(eUart_Debug, message, strlen(message));
    if (osMutexRelease(debug_mutex_id) != osOK) {
        return false;
    }
    return status;
}

