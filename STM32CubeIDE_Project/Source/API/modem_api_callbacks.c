/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "modem_api_callbacks.h"
#include "string_utils.h"
#include "modem_api.h"

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
#define BAD_CHARS " "
#define SEPARATOR ","

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
static char *token = NULL;

/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
static int Modem_API_ParseInt (char *buffer, char *separator) {
    if (StrContains(buffer, strlen(buffer), separator) == true) {
        token = strtok(buffer, separator);
    } else {
        token = strtok(NULL, separator);
    }
    return atoi(token);
}
static char* Modem_API_ParseString (char *buffer, char *separator) {
    if (StrContains(buffer, strlen(buffer), separator) == true) {
        token = strtok(buffer, separator);
    } else {
        token = strtok(NULL, separator);
    }
    return token;
}

/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
/* DEFAULT */
bool Modem_API_OK_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    Modem_API_SetFlag(eModemFlag_MsgRecieved, eModemApiState_OK);
    snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
    "OK");
    Modem_API_PutResponse(eModemApiCmd_OK, cmd_api_callback_args->response_buffer);
    return true;
}
bool Modem_API_ERROR_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    Modem_API_SetFlag(eModemFlag_MsgRecieved, eModemApiState_ERROR);
    snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
    "ERROR");
    Modem_API_PutResponse(eModemApiCmd_OK, cmd_api_callback_args->response_buffer);
    return true;
}

/* SETUP */
bool Modem_API_EIND_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    StrRemChar(cmd_api_callback_args->arguments, BAD_CHARS);
    if (strcmp(cmd_api_callback_args->arguments, "1") == 0) {
        Modem_API_SetFlag(eModemFlag_Ready, eModemApiState_Ready);
        snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
        "Modem is ready.");
    } else {
        strncpy(cmd_api_callback_args->response_buffer, "", cmd_api_callback_args->response_buffer_length);
    }
    Modem_API_PutResponse(eModemApiCmd_EIND, cmd_api_callback_args->arguments);
    return true;
}
bool Modem_API_EUSIM_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    StrRemChar(cmd_api_callback_args->arguments, BAD_CHARS);
    if (strcmp(cmd_api_callback_args->arguments, "1") == 0) {
        Modem_API_SetFlag(eModemFlag_Sim1, eModemApiState_Ready);
    } else if (strcmp(cmd_api_callback_args->arguments, "2") == 0) {
        Modem_API_SetFlag(eModemFlag_Sim2, eModemApiState_Ready);
    }

    snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
    "SIM slot: %s", cmd_api_callback_args->arguments);
    return true;
}
bool Modem_API_ECHO_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
    "Echo disabled.");
    return true;
}

/* TCP IP SETUP */
bool Modem_API_CGDCONT_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
    "PDP context parameters:%s", cmd_api_callback_args->arguments);
    Modem_API_PutResponse(eModemApiCmd_CGDCONT, cmd_api_callback_args->arguments);
    return true;
}
bool Modem_API_EGDCONT_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
    "TCP/IP data account parameters:%s", cmd_api_callback_args->arguments);
    Modem_API_PutResponse(eModemApiCmd_EGDCONT, cmd_api_callback_args->arguments);
    return true;
}
bool Modem_API_CGATT_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
    "MT state:%s", cmd_api_callback_args->arguments);
    if (strcmp(cmd_api_callback_args->arguments, " 1") == 0) {
        Modem_API_SetFlag(eModemFlag_PdpAttach, eModemApiState_Ready);
    } else {
        Modem_API_ClearFlag(eModemFlag_PdpAttach, eModemApiState_NotReady);
    }
    Modem_API_PutResponse(eModemApiCmd_CGATT, cmd_api_callback_args->arguments);
    return true;
}
bool Modem_API_CGACT_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    int cid = Modem_API_ParseInt(cmd_api_callback_args->arguments, SEPARATOR);
    int state = Modem_API_ParseInt(cmd_api_callback_args->arguments, SEPARATOR);
    snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
    "TCP/IP CID-%d; state:%s", cid, (state == 1) ? "Activated" : "Deactivated");
    if (state == 1) {
        Modem_API_SetFlag(eModemFlag_PdpActive, eModemApiState_Ready);
        Modem_API_PutResponse(eModemApiCmd_CGACT, "Activated");
    } else {
        Modem_API_ClearFlag(eModemFlag_PdpActive, eModemApiState_NotReady);
        Modem_API_PutResponse(eModemApiCmd_CGACT, "Deactivated");
    }
    return true;
}
bool Modem_API_CGPADDR_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
    "TCP/IP address:%s", cmd_api_callback_args->arguments);
    return true;
}
bool Modem_API_ETL_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
    "Socket:%s", cmd_api_callback_args->arguments);
    Modem_API_PutResponse(eModemApiCmd_ETL, cmd_api_callback_args->arguments);
    return true;
}
/* TCP IP SEND/RECIEVE */
bool Modem_API_EIPSEND_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
    "Data length:%s", cmd_api_callback_args->arguments);
    return true;
}
bool Modem_API_ESOCK_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    StrRemChar(cmd_api_callback_args->arguments, ",");
    snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
    "Socket:%s ", cmd_api_callback_args->arguments);

    /* RESPONSE */
    if (strstr(cmd_api_callback_args->arguments, "RECV") != NULL) {
        char *socket_id = Modem_API_ParseString(cmd_api_callback_args->arguments, " ");
        Modem_API_PutResponse(eModemApiCmd_ESOCK, socket_id);
        Modem_API_SetFlag(eModemFlag_UdpRecieved, eModemApiState_Ready);
    }

    return true;
}
bool Modem_API_EIPRECV_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    char *socket_id = Modem_API_ParseString(cmd_api_callback_args->arguments, ",");
    char *data = Modem_API_ParseString(cmd_api_callback_args->arguments, ",");
    snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
    "Socket:%s  DataRecieved: %s", socket_id, data);

    Modem_API_PutResponse(eModemApiCmd_EIPRECV, data);
    Modem_API_SetFlag(eModemFlag_UdpRecieved, eModemApiState_OK);
    return true;
}

/* SMS */
bool Modem_API_MsgSent_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
    "Message sent.");
    return true;
}
bool Modem_API_MsgRef_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    strncpy(cmd_api_callback_args->response_buffer, "", cmd_api_callback_args->response_buffer_length);
    Modem_API_PutResponse(eModemApiCmd_MsgRef, cmd_api_callback_args->arguments);
    return true;
}
bool Modem_API_CPMS_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
    "Message Storage (stored/total):%s", cmd_api_callback_args->arguments);
    Modem_API_PutResponse(eModemApiCmd_CPMS, cmd_api_callback_args->arguments);
    return true;
}
/* SMS READ */
bool Modem_API_CMTI_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    Modem_API_SetFlag(eModemFlag_SmsRecieved, eModemApiState_Ready);
    StrRemChar(cmd_api_callback_args->arguments, BAD_CHARS);

    Modem_API_ParseString(cmd_api_callback_args->arguments, SEPARATOR);
    char *sms_num = Modem_API_ParseString(cmd_api_callback_args->arguments, SEPARATOR);
    snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
    "New SMS id:%s", sms_num);
    Modem_API_PutResponse(eModemApiCmd_CMTI, sms_num);
    return true;
}
bool Modem_API_CMGL_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    Modem_API_ClearFlag(eModemFlag_SmsRead, eModemApiState_Ready);
    snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
    "SMS info:%s", cmd_api_callback_args->arguments);
    Modem_API_PutResponse(eModemApiCmd_CMGL, cmd_api_callback_args->arguments);
    return true;
}
bool Modem_API_CMGR_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    Modem_API_ClearFlag(eModemFlag_SmsRead, eModemApiState_Ready);
    snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
    "SMS info:%s", cmd_api_callback_args->arguments);
    Modem_API_PutResponse(eModemApiCmd_CMGL, cmd_api_callback_args->arguments);
    return true;
}

bool Modem_API_Start_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
    "Message received: %s", cmd_api_callback_args->arguments);
    return true;
}
bool Modem_API_Stop_Callback (sCmdApi_CallbackArgs_t *cmd_api_callback_args) {
    snprintf(cmd_api_callback_args->response_buffer, cmd_api_callback_args->response_buffer_length,
    "Message received: %s", cmd_api_callback_args->arguments);
    return true;
}
