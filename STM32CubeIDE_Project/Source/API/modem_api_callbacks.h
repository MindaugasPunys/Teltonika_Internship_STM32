#ifndef __MODEM_API_CALLBACKS__H__
#define __MODEM_API_CALLBACKS__H__

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
bool Modem_API_OK_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);
bool Modem_API_ERROR_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);

bool Modem_API_EIND_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);
bool Modem_API_EUSIM_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);
bool Modem_API_ECHO_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);

bool Modem_API_CGDCONT_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);
bool Modem_API_EGDCONT_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);
bool Modem_API_CGATT_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);
bool Modem_API_CGACT_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);
bool Modem_API_CGPADDR_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);
bool Modem_API_ETL_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);

bool Modem_API_EIPSEND_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);
bool Modem_API_ESOCK_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);
bool Modem_API_EIPRECV_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);

bool Modem_API_MsgSent_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);
bool Modem_API_MsgRef_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);
bool Modem_API_CPMS_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);
bool Modem_API_CMTI_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);
bool Modem_API_CMGL_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);
bool Modem_API_CMGR_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);

bool Modem_API_Start_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);
bool Modem_API_Stop_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args);

#endif /* __SOMETHING__H__ */
