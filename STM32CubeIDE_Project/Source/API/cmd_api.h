#ifndef __CMD_API__H__
#define __CMD_API__H__

/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "stdbool.h"
#include "stdint.h"
/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/
typedef struct sCmdApi_CallbackArgs_t {
    char *arguments;
    uint16_t arguments_length;
    char *response_buffer;
    uint16_t response_buffer_length;
} sCmdApi_CallbackArgs_t;

typedef struct sCmdApiStaticLut_t {
    const char *cmd_name;
    bool (*cmd_callback) (sCmdApi_CallbackArgs_t*);
} sCmdApiStaticLut_t;
/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/
typedef struct sCmdApiParserArgs_t {
    char *message;
    uint16_t message_length;

    char *response_buffer;
    uint16_t response_buffer_length;

    uint16_t static_lut_length;
    const sCmdApiStaticLut_t *static_lut;
} sCmdApiParserArgs_t;

/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
bool CMD_API_ParseCommand (sCmdApiParserArgs_t *cmd_api_args, char *separator);

#endif /* __CMD_API__H__ */
