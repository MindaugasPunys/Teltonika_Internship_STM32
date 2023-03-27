/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "cmd_api.h"
#include "debug_api.h"
#include "string_utils.h"
#include "cli_app.h"

#include "stddef.h"
#include "string.h"
#include "stdio.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define SEPARATOR ":\n"
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
//INIT_DEBUG_MODULE(CMD_API)
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
bool CMD_API_ParseCommand (sCmdApiParserArgs_t *cmd_api_args, char *separator) {
    /* ASSERT */
    if (cmd_api_args->message == NULL) {
        snprintf(cmd_api_args->response_buffer, (int) cmd_api_args->message_length,
        static_debug_message_lut[eDebugMsg_ArgumentEmpty]);
        return false;
    }

    /* FIND CALLBACK */
    char *token = NULL;
    bool following_args = StrContains(cmd_api_args->message, cmd_api_args->message_length, separator);
    token = strtok(cmd_api_args->message, separator);
    for (uint16_t task = 0; task < cmd_api_args->static_lut_length; task++) {
        if (StrCompare(token, cmd_api_args->static_lut[task].cmd_name) == true) {
            if (following_args == true) {
                token = strtok(NULL, separator);
            }
            cmd_api_args->message = token;
            sCmdApi_CallbackArgs_t callback_arguments = {
                .arguments = cmd_api_args->message,
                .arguments_length = cmd_api_args->message_length,
                .response_buffer = cmd_api_args->response_buffer,
                .response_buffer_length = cmd_api_args->response_buffer_length,
            };

            if (cmd_api_args->static_lut[task].cmd_callback(&callback_arguments) == false) {
                return false;
            } else {
                return true;
            }
        }
    }

    /* CALLBACK NOT FOUND */
    if (token != NULL) {
        snprintf(cmd_api_args->response_buffer, cmd_api_args->response_buffer_length,
        "Command \"%s\" not found", token);
    }
    return false;
}
