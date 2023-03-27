/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "sms_app.h"
#include "debug_api.h"
#include "modem_api.h"
#include "string_utils.h"
#include "heap_api.h"

#include "cli_app.h"
#include "cmd_api.h"
#include "network_app.h"

#include "cmsis_os2.h"
#include "string.h"
#include "stdio.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define CID 1
#define PDP_TYPE "IP"
#define DEST_IP "88.119.190.245"
#define DEST_PORT 10034
//#define APN "internet" // Bitė APN
#define APN "banga" // Bitė APN

#define SEPARATOR ":=\n"
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
INIT_DEBUG_MODULE(SMS_APP)
static const osThreadAttr_t sms_app_task_attributes = {.name = "SmsApp task", .priority = (osPriority_t) osPriorityBelowNormal, .stack_size = 1024};

/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
static osThreadId_t sms_app_task_handle;
static sCmdApiParserArgs_t cmd_api_args = {0};

static char buffer[SMS_LENGTH] = {0};
static char response_buffer[SMS_LENGTH] = {0};
bool sms_app_setup = true;
/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/

static bool SMS_APP_Setup (void) {
    /* ECHO OFF */
    if (Modem_API_GetState(eModemFlag_Echo) != eModemApiState_OFF &&
        SMS_APP_SendAndSetFlag("ATE0\n", eModemFlag_Echo, eModemApiState_OFF, DEFAULT_WAIT_TIME) == false) {
        return false;
    }
    /* TEXT MODE */
    if (Modem_API_GetState(eModemFlag_TextMode) != eModemApiState_Ready &&
        SMS_APP_SendAndSetFlag("AT+CMGF=1\n", eModemFlag_TextMode, eModemApiState_Ready, DEFAULT_WAIT_TIME) == false) {
        return false;
    }
    /* SMS STORAGE SETUP */
    if (Modem_API_GetState(eModemFlag_SmsStore) != eModemApiState_Ready) {
        /* SMS store location SIM */
        if (SMS_APP_SendAndSetFlag("AT+CPMS=\"SM\",\"SM\",\"SM\"\n", eModemFlag_SmsStore, eModemApiState_NotReady, DEFAULT_WAIT_TIME) == false) {
            return false;
        }
        /* Notify about new SMS id */
        if (SMS_APP_SendAndSetFlag("AT+CNMI=2,1,0,1,0\n", eModemFlag_SmsStore, eModemApiState_Ready, DEFAULT_WAIT_TIME) == false) {
            return false;
        }
    }

    /* SET PDP CONTEXT */
    if (Modem_API_GetState(eModemFlag_PdpContext) != eModemApiState_Ready) {
        sprintf(buffer, "AT+CGDCONT=%d,\"%s\",\"%s\"\n", CID, PDP_TYPE, APN);
        if (SMS_APP_SendAndSetFlag(buffer, eModemFlag_PdpContext, eModemApiState_Ready, DEFAULT_WAIT_TIME) == false) {
            return false;
        }
    }
    /* SET TCP CONTEXT */
    if (Modem_API_GetState(eModemFlag_TcpContext) != eModemApiState_Ready) {
        sprintf(buffer, "AT+EGDCONT=%d,\"%s\",\"%s\"\n", DATA_ACC_ID, PDP_TYPE, APN);
        if (SMS_APP_SendAndSetFlag(buffer, eModemFlag_TcpContext, eModemApiState_Ready, DEFAULT_WAIT_TIME) == false) {
            return false;
        }
    }
//    Modem_API_SetFlag(eModemFlag_Setup, eModemApiState_Ready);
    return true;
}
/* SMS */
static bool SMS_APP_SendSms (char *number, char *message) {
    sprintf(buffer, "AT+CMGS=\"%s\"\r", number);
    Modem_API_Send(buffer);
    sprintf(buffer, "%s%c\n", message, 26);
    return Modem_API_SendAndWait(buffer, 60 * SECOND_IN_MS, 1);
}
static bool SMS_APP_ReadSms (void) {
    char id[3] = {0};
    Modem_API_GetResponse(eModemApiCmd_CMTI, (char*) id);
    sprintf(buffer, "AT+CMGR=%s\n", id);
    Modem_API_SendAndWait(buffer, SECOND_IN_MS, 1);
    sprintf(buffer, "AT+CMGD=%s\n", id);
    Modem_API_SendAndWait(buffer, SECOND_IN_MS, 1);
    return true;
}
static bool SMS_APP_ParseSms (void) {
    strncpy(buffer, "", SMS_LENGTH);
    Modem_API_GetResponse(eModemApiCmd_CMTI, buffer);

    uint16_t sms_array_size = StrCountChars(buffer, SMS_LENGTH, "\n");
    char *sms_array[sms_array_size];
    char *token = strtok(buffer, "\n");
    uint16_t token_length = 0;
    for (uint16_t i = 0; i < sms_array_size; i++) {
        token_length = strlen(token);
        sms_array[i] = Heap_API_Allocate(token_length + 1, sizeof(char));
        strncpy(sms_array[i], token, token_length);
        token = strtok(NULL, "\n");
    }

    for (uint16_t i = 0; i < sms_array_size; i++) {
        cmd_api_args.message = sms_array[i];
        cmd_api_args.message_length = SMS_LENGTH;
        cmd_api_args.response_buffer = response_buffer;
        cmd_api_args.response_buffer_length = SMS_LENGTH;
        cmd_api_args.static_lut_length = (uint16_t) eCliAppCmd_Last;
        cmd_api_args.static_lut = (sCmdApiStaticLut_t*) &static_cli_app_lut;
        CMD_API_ParseCommand(&cmd_api_args, SEPARATOR);

        if (strcmp(cmd_api_args.response_buffer, "") != 0) {
            debug("%s\n", cmd_api_args.response_buffer);
        }
        Heap_API_Free(sms_array[i]);
    };

    return true;
}
static bool SMS_APP_ReadAllSms (void) {
    return Modem_API_SendAndWait("AT+CMGL=\"REC UNREAD\"\n", SECOND_IN_MS, 1);
}

/* THREAD */
static void SMS_APP_Thread (void *argument) {
    while (true) {
        if (sms_app_setup == true) {
            /* READY */
            if (Modem_API_WaitFlag(eModemFlag_Ready, DEFAULT_WAIT_TIME) == true) {
                sms_app_setup = !SMS_APP_Setup();
            }
        } else {
            switch (Modem_API_GetState(eModemFlag_SmsRecieved)) {
                /* SMS APP READY */
                case eModemApiState_Unknown: {
                    modem("SMS APP SETUP DONE.\n");
//                    SMS_APP_SendSms("+37065828368", "Beep Boop. Hurr durr I am a tracker.");

                    Modem_API_ClearFlag(eModemFlag_SmsRecieved, eModemApiState_NotReady);
                    Modem_API_SetFlag(eModemFlag_Setup, eModemApiState_Ready);
                    break;
                }

                    /* DEFAULT */
                case eModemApiState_NotReady: {
                    Modem_API_WaitFlag(eModemFlag_SmsRecieved, osWaitForever);
                    break;
                }

                    /* READ SMS CONTENTS */
                case eModemApiState_Ready: {
                    Modem_API_ClearFlag(eModemFlag_SmsRecieved, eModemApiState_NotReady);
                    SMS_APP_ReadSms();
                    break;
                }

                    /* PARSE SMS CONTENTS */
                case eModemApiState_OK: {
                    SMS_APP_ParseSms();
                    Modem_API_ClearFlag(eModemFlag_SmsRecieved, eModemApiState_NotReady);
                    break;
                }

                default: {
                    Modem_API_ClearFlag(eModemFlag_SmsRead, eModemApiState_NotReady);
                    break;
                }
            }
        }
    }
    osThreadTerminate(NULL);
}
/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool SMS_APP_Init (void) {
    /* THREAD */
    sms_app_task_handle = osThreadNew(SMS_APP_Thread, NULL, &sms_app_task_attributes);
    if (sms_app_task_handle == NULL) {
        return false;
    }
    return true;
}
