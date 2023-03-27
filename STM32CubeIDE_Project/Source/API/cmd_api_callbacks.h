#ifndef __CMD_API_CALLBACKS__H__
#define __CMD_API_CALLBACKS__H__

/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "cmd_api.h"
#include "stdbool.h"
#include "stdint.h"
/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
bool CMD_API_SetCallback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);
bool CMD_API_ResetCallback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);
bool CMD_API_ToggleCallback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);
bool CMD_API_BlinkCallback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);

bool CMD_API_ModemCallback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);
bool CMD_API_ModemStartTcpCallback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);
bool CMD_API_ModemStopTcpCallback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);
#endif /* __CMD_API_CALLBACKS__H__ */
