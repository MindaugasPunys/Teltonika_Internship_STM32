#ifndef __DEBUG_API__H__
#define __DEBUG_API__H__

/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "stdbool.h"
#include "stdarg.h"

/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/
#define MODEM_MODULE "__MODEM_MODULE__"
#define INIT_DEBUG_MODULE(module) static const char *MODULE = #module;
#define debug(...) Debug_API_Print(MODULE, NULL, 0, __VA_ARGS__)
#define error(...) Debug_API_Print(MODULE, __FILE__, __LINE__, __VA_ARGS__)
#define modem(...) Debug_API_Print(MODEM_MODULE, NULL, 0, __VA_ARGS__)

/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/
// TO DO IMPLEMENT ERROR LUT AND FUN TO ADD ARGS?
typedef enum eDebugMessage_t {
    eDebugMsg_First = 0,
        eDebugMsg_ArgumentBad = eDebugMsg_First,
        eDebugMsg_ArgumentBadSpecify,
        eDebugMsg_MessageQueuePutFailed,
        eDebugMsg_MessageQueueGetFailed,
        eDebugMsg_ArgumentEmpty,
        eDebugMsg_ArgumentNotFound,
        eDebugMsg_Last
} eDebugMessage_t;
/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/
extern const char *static_debug_message_lut[eDebugMsg_Last];
/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
bool Debug_API_Init ();
bool Debug_API_Print (const char *module, const char *file_path, const int line, const char *format, ...);
#endif /* __SOMETHING__H__ */
