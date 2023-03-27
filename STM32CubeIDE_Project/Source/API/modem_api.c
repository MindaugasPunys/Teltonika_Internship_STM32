/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "modem_api.h"
#include "gpio_driver.h"
#include "uart_api.h"
#include "debug_api.h"
#include "heap_api.h"
#include "message.h"
#include "string_utils.h"

#include "cmd_api.h"
#include "modem_api_callbacks.h"

#include "string.h"
#include "stdio.h"
#include "cmsis_os2.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define RESPONSE_BUFFER_LENGTH 128
#define SMS_LENGTH 160
#define SMS_READ_TIME 200
#define RETRY_COUNT 5
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
#define SEPARATOR ":=\n"

typedef struct sModemApiDynamic_t {
    eModemApiStateEnum_t state;
    osEventFlagsId_t event_flag_id;
    uint32_t event_flag_mask;
} sModemApiDynamic_t;

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
INIT_DEBUG_MODULE(MODEM_API)
static const osThreadAttr_t modem_api_task_attributes = {.name = "ModemApi task", .priority = (osPriority_t) osPriorityNormal, .stack_size = 1024};
static const osMutexAttr_t mutex_attributes = {"modem_api_mutex", 0, NULL, 0U};
// @formatter:off
static const sCmdApiStaticLut_t static_modem_api_lut[eModemApiCmd_Last] = {
    [eModemApiCmd_OK] =         {.cmd_name = "OK",          .cmd_callback = Modem_API_OK_Callback},
    [eModemApiCmd_ERROR] =      {.cmd_name = "ERROR",       .cmd_callback = Modem_API_ERROR_Callback},

    [eModemApiCmd_ECHO] =       {.cmd_name = "ATE0",        .cmd_callback = Modem_API_ECHO_Callback},
    [eModemApiCmd_EIND] =       {.cmd_name = "+EIND",       .cmd_callback = Modem_API_EIND_Callback},
    [eModemApiCmd_EUSIM] =      {.cmd_name = "+EUSIM",      .cmd_callback = Modem_API_EUSIM_Callback},

    [eModemApiCmd_MsgSent] =    {.cmd_name = "> ",          .cmd_callback = Modem_API_MsgSent_Callback},
    [eModemApiCmd_MsgRef] =     {.cmd_name = "+CMGS",       .cmd_callback = Modem_API_MsgRef_Callback},
    [eModemApiCmd_CPMS] =       {.cmd_name = "+CPMS",       .cmd_callback = Modem_API_CPMS_Callback},
    [eModemApiCmd_CMTI] =       {.cmd_name = "+CMTI",       .cmd_callback = Modem_API_CMTI_Callback},
    [eModemApiCmd_CMGL] =       {.cmd_name = "+CMGL",       .cmd_callback = Modem_API_CMGL_Callback},
    [eModemApiCmd_CMGR] =       {.cmd_name = "+CMGR",       .cmd_callback = Modem_API_CMGR_Callback},

    [eModemApiCmd_Start] =      {.cmd_name = "Start",       .cmd_callback = Modem_API_Start_Callback},
    [eModemApiCmd_Stop] =       {.cmd_name = "Stop",        .cmd_callback = Modem_API_Stop_Callback},

    [eModemApiCmd_CGDCONT] =    {.cmd_name = "+CGDCONT",    .cmd_callback = Modem_API_CGDCONT_Callback},
    [eModemApiCmd_EGDCONT] =    {.cmd_name = "+EGDCONT",    .cmd_callback = Modem_API_EGDCONT_Callback},
    [eModemApiCmd_CGATT] =      {.cmd_name = "+CGATT",      .cmd_callback = Modem_API_CGATT_Callback},
    [eModemApiCmd_CGACT] =      {.cmd_name = "+CGACT",      .cmd_callback = Modem_API_CGACT_Callback},
    [eModemApiCmd_CGPADDR] =    {.cmd_name = "+CGPADDR",    .cmd_callback = Modem_API_CGPADDR_Callback},
    [eModemApiCmd_ETL] =        {.cmd_name = "+ETL",        .cmd_callback = Modem_API_ETL_Callback},

    [eModemApiCmd_EIPSEND] =    {.cmd_name = "+EIPSEND",    .cmd_callback = Modem_API_EIPSEND_Callback},
    [eModemApiCmd_ESOCK] =      {.cmd_name = "+ESOCK",      .cmd_callback = Modem_API_ESOCK_Callback},
    [eModemApiCmd_EIPRECV] =    {.cmd_name = "+EIPRECV",    .cmd_callback = Modem_API_EIPRECV_Callback},
};
// @formatter:on

/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
static osThreadId_t modem_api_task_handle;
static osMutexId_t modem_mutex_id = NULL;
static osEventFlagsId_t evt_id = NULL;
static osTimerId_t timer_id = NULL;

static char sms_message[RESPONSE_BUFFER_LENGTH] = {0};
static char modem_api_response_buffer[RESPONSE_BUFFER_LENGTH] = {0};
static sMessage_t modem_message = {0};
static sCmdApiParserArgs_t cmd_api_args = {0};

static sMessage_t dynamic_modem_api_lut[eModemApiCmd_Last] = {0};
// @formatter:off
// REMOVE REPATED FLAG ID
static sModemApiDynamic_t modem_api_flag_lut[eModemFlag_Last] = {
    /* GENERAL */
    [eModemFlag_Ready] =            {.state = eModemApiState_Unknown, .event_flag_id = NULL, .event_flag_mask = 0x00000001U},
    [eModemFlag_MsgRecieved] =      {.state = eModemApiState_Unknown, .event_flag_id = NULL, .event_flag_mask = 0x00000002U},
    [eModemFlag_Echo] =             {.state = eModemApiState_Unknown, .event_flag_id = NULL, .event_flag_mask = 0x00000004U},
    [eModemFlag_Setup] =            {.state = eModemApiState_Unknown, .event_flag_id = NULL, .event_flag_mask = 0x00000008U},
    /* SMS */
    [eModemFlag_Sim1] =             {.state = eModemApiState_Unknown, .event_flag_id = NULL, .event_flag_mask = 0x00000010U},
    [eModemFlag_Sim2] =             {.state = eModemApiState_Unknown, .event_flag_id = NULL, .event_flag_mask = 0x00000020U},
    [eModemFlag_TextMode] =         {.state = eModemApiState_Unknown, .event_flag_id = NULL, .event_flag_mask = 0x00000040U},
    [eModemFlag_SmsStore] =         {.state = eModemApiState_Unknown, .event_flag_id = NULL, .event_flag_mask = 0x00000080U},
    [eModemFlag_SmsRead] =          {.state = eModemApiState_Unknown, .event_flag_id = NULL, .event_flag_mask = 0x00000100U},
    [eModemFlag_SmsRecieved] =      {.state = eModemApiState_Unknown, .event_flag_id = NULL, .event_flag_mask = 0x00000200U},
    /* GPRS TCP IP */
    [eModemFlag_PdpContext] =       {.state = eModemApiState_Unknown, .event_flag_id = NULL, .event_flag_mask = 0x00001000U},
    [eModemFlag_PdpAttach] =        {.state = eModemApiState_Unknown, .event_flag_id = NULL, .event_flag_mask = 0x00002000U},
    [eModemFlag_PdpActive] =        {.state = eModemApiState_Unknown, .event_flag_id = NULL, .event_flag_mask = 0x00004000U},
    [eModemFlag_TcpContext] =       {.state = eModemApiState_Unknown, .event_flag_id = NULL, .event_flag_mask = 0x00008000U},
    [eModemFlag_TcpSocket] =        {.state = eModemApiState_Unknown, .event_flag_id = NULL, .event_flag_mask = 0x00010000U},
    [eModemFlag_TcpRecieved] =      {.state = eModemApiState_Unknown, .event_flag_id = NULL, .event_flag_mask = 0x00020000U},
    [eModemFlag_UdpSocket] =        {.state = eModemApiState_Unknown, .event_flag_id = NULL, .event_flag_mask = 0x00040000U},
    [eModemFlag_UdpRecieved] =      {.state = eModemApiState_Unknown, .event_flag_id = NULL, .event_flag_mask = 0x00080000U},
};
// @formatter:on
/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/
static bool Modem_API_ParseMessage (char *string);
/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
/* SMS HANDLING */
static void Modem_API_EndOfSmsCallback (void *argument) {
    char *last_line = StrLastLine(sms_message);
    if (strcmp(last_line, "OK\n") == 0) {
        Modem_API_ParseMessage("OK");
    } else {
        Modem_API_ParseMessage("ERROR");
    }
    StrRemSubstring(sms_message, "ERROR\n");
    StrRemSubstring(sms_message, "OK\n");

    Modem_API_PutResponse(eModemApiCmd_CMTI, sms_message);
    strncpy(sms_message, "", SMS_LENGTH);
    Modem_API_SetFlag(eModemFlag_SmsRecieved, eModemApiState_OK);
    Modem_API_ClearFlag(eModemFlag_SmsRead, eModemApiState_NotReady);
}
static bool Modem_API_ParseSms (void) {
    modem("SMS: %s", modem_message.buffer);
    strcat(sms_message, modem_message.buffer);
    if (osTimerStart(timer_id, SMS_READ_TIME) != osOK) {
        error("Failed to start timer!\n");
        return false;
    }
    return true;
}
static bool Modem_API_ParseMessage (char *string) {
    if (string == NULL) {
        cmd_api_args.message = modem_message.buffer;
        cmd_api_args.message_length = modem_message.length;
    } else {
        cmd_api_args.message = string;
        cmd_api_args.message_length = strlen(string);
    }

    cmd_api_args.response_buffer = modem_api_response_buffer;
    cmd_api_args.response_buffer_length = RESPONSE_BUFFER_LENGTH;
    cmd_api_args.static_lut_length = (uint16_t) eModemApiCmd_Last;
    cmd_api_args.static_lut = (sCmdApiStaticLut_t*) &static_modem_api_lut;

    CMD_API_ParseCommand(&cmd_api_args, SEPARATOR);
    if (*cmd_api_args.response_buffer != '\0') {
        modem("%s\n", cmd_api_args.response_buffer);
        return true;
    } else {
        return false;
    }
}

/* THREAD */
static void Modem_API_Thread (void *argument) {
    while (true) {
        if (UART_API_GetMessage(&modem_message, eUart_Modem) == true) {
            if (modem_message.length != 1 && modem_message.buffer[0] != '\n') {
//                modem("\t\t\t\t\t\t%s", modem_message.buffer); // FOR TESTING PURPOSES
//                debug("mem:%u\n", Heap_API_BlockCount()); // FOR TESTING PURPOSES
                switch (Modem_API_GetState(eModemFlag_SmsRead)) {

                    case eModemApiState_NotReady: {
                        Modem_API_ParseMessage(NULL);
                        break;
                    }

                    case eModemApiState_Ready: {
                        Modem_API_ParseSms();
                        break;
                    }

                    default: {
                        Modem_API_ClearFlag(eModemFlag_SmsRead, eModemApiState_NotReady);
                        break;
                    }
                }
            }

            if (modem_message.buffer != NULL) {
                Heap_API_Free(modem_message.buffer);
            }
        }
    }
    osThreadTerminate(NULL);
}

static bool Modem_API_InitGpio (void) {
    if (GPIO_Driver_Init(eGpioDriver_ModemONPin) == false) { // MODEM_ON
        return false;
    };
    if (GPIO_Driver_Init(eGpioDriver_ModemPowerOffPin) == false) { // Reset_N
        return false;
    };
    if (GPIO_Driver_Init(eGpioDriver_GNSSOnPin) == false) { // Reset_N
        return false;
    };

    if (GPIO_Driver_SetPin(eGpioDriver_ModemONPin) == false) {
        return false;
    }
    if (GPIO_Driver_SetPin(eGpioDriver_ModemPowerOffPin) == false) {
        return false;
    }
    if (GPIO_Driver_SetPin(eGpioDriver_GNSSOnPin) == false) {
        return false;
    }
    return true;
}
/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool Modem_API_Init (void) {
    /* GPIO */
    if (Modem_API_InitGpio() == false) {
        return false;
    }
    UART_API_Init(eUart_Modem, "\n\r", eUartBaudrate_Default);

    /* MUTEX */
    modem_mutex_id = osMutexNew(&mutex_attributes);
    if (modem_mutex_id == NULL) {
        error("Failed to create MUTEX\n");
        return false;
    }
    /* EVENT FLAGS */
    evt_id = osEventFlagsNew(NULL);
    if (evt_id == NULL) {
        osMutexDelete(modem_mutex_id);
        error("Failed to create event flags\n");
        return false;
    }
    for (uint8_t flag = eModemFlag_First; flag < eModemFlag_Last; flag++) {
        modem_api_flag_lut[flag].event_flag_id = evt_id;
    }
    /* TIMER */
    timer_id = osTimerNew(Modem_API_EndOfSmsCallback,
    osTimerOnce, NULL, NULL);
    if (timer_id == NULL) {
        osMutexDelete(modem_mutex_id);
        osEventFlagsDelete(evt_id);
        error("Failed to create event flags\n");
        return false;
    }
    /* THREAD */
    modem_api_task_handle = osThreadNew(Modem_API_Thread, NULL, &modem_api_task_attributes);
    if (modem_api_task_handle == NULL) {
        osMutexDelete(modem_mutex_id);
        osEventFlagsDelete(evt_id);
        osTimerDelete(timer_id);
        return false;
    }

    return true;
}

/* RESPONSE */
bool Modem_API_PutResponse (eModemApiCommandEnum_t response_enum, char *response) {
    if (response_enum >= eModemApiCmd_Last || response == NULL) {
        return false;
    }

    if (dynamic_modem_api_lut[response_enum].buffer != NULL) {
        dynamic_modem_api_lut[response_enum].length = 0;
        Heap_API_Free(dynamic_modem_api_lut[response_enum].buffer);
    }
    dynamic_modem_api_lut[response_enum].length = strlen(response);
    dynamic_modem_api_lut[response_enum].buffer = Heap_API_Allocate(dynamic_modem_api_lut[response_enum].length + 1, sizeof(char));

    strcpy(dynamic_modem_api_lut[response_enum].buffer, response);

    return true;
}
bool Modem_API_GetResponse (eModemApiCommandEnum_t response_enum, char *response) {
    if (response_enum >= eModemApiCmd_Last || response == NULL) {
        return false;
    }
    if (strcmp(dynamic_modem_api_lut[response_enum].buffer, "\0") != 0) {
        strncpy(response, dynamic_modem_api_lut[response_enum].buffer, dynamic_modem_api_lut[response_enum].length);
        Heap_API_Free(dynamic_modem_api_lut[response_enum].buffer);
        dynamic_modem_api_lut[response_enum].buffer = NULL;
        dynamic_modem_api_lut[response_enum].length = 0;
    }
    return true;
}

/* SEND */
bool Modem_API_SendAndWait (char *command_buffer, uint32_t timeout, uint32_t try_count) {
    /* MUTEX */
    if (osMutexAcquire(modem_mutex_id, timeout) != osOK) {
        return false;
    }

    /* CLEAR FLAG */
    uint32_t flag_status = osFlagsErrorUnknown;
    flag_status = osEventFlagsClear(modem_api_flag_lut[eModemFlag_MsgRecieved].event_flag_id, modem_api_flag_lut[eModemFlag_MsgRecieved].event_flag_mask);
    /* ASSERT */
    if (flag_status == osFlagsErrorUnknown || flag_status == osFlagsErrorParameter || flag_status == osFlagsErrorResource) {
        error("Failed to clear message flag.\n");
        osMutexRelease(modem_mutex_id);
        return false;
    }

    for (uint16_t i = 0; i < try_count; i++) {
        /* SEND */
        modem("%s", command_buffer);
        UART_API_Send(eUart_Modem, command_buffer);
        flag_status = osEventFlagsWait(modem_api_flag_lut[eModemFlag_MsgRecieved].event_flag_id, modem_api_flag_lut[eModemFlag_MsgRecieved].event_flag_mask, osFlagsWaitAny, timeout);
        /* ASSERT */
        if (flag_status == osFlagsErrorUnknown || flag_status == osFlagsErrorParameter || flag_status == osFlagsErrorResource) {
            osMutexRelease(modem_mutex_id);
            return false;
        }
        /* TIMEOUT */
        if (flag_status == osFlagsErrorTimeout) {
            osMutexRelease(modem_mutex_id);
            return false;
        }
        /* MESSAGE RECIEVED */
        if (Modem_API_GetState(eModemFlag_MsgRecieved) != eModemApiState_ERROR) {
            break;
        } else {
            osDelay(timeout);
        }
    }

    if (osMutexRelease(modem_mutex_id) != osOK) {
        return false;
    }
    return true;
}
bool Modem_API_Send (char *command_buffer) {
    if (osMutexAcquire(modem_mutex_id, osWaitForever) != osOK) {
        return false;
    }

    /* CLEAR FLAG */
    uint32_t flag_status = osFlagsErrorUnknown;
    flag_status = osEventFlagsClear(modem_mutex_id, modem_api_flag_lut[eModemFlag_MsgRecieved].event_flag_mask);
    /* ASSERT */
    if (flag_status == osFlagsErrorUnknown || flag_status == osFlagsErrorParameter || flag_status == osFlagsErrorResource) {
        osMutexRelease(modem_mutex_id);
        return false;
    }
    /* SEND */
    bool status = UART_API_Send(eUart_Modem, command_buffer);
    modem("%s", command_buffer);
    if (osMutexRelease(modem_mutex_id) != osOK) {
        return false;
    }
    return status;
}

/* FLAGS */
bool Modem_API_SetFlag (eModemFlagsEnum_t flag, eModemApiStateEnum_t state) {
    uint32_t flag_status = osFlagsErrorUnknown;
    flag_status = osEventFlagsSet(modem_api_flag_lut[eModemFlag_MsgRecieved].event_flag_id, modem_api_flag_lut[flag].event_flag_mask);
    if (flag_status == osFlagsErrorUnknown || flag_status == osFlagsErrorParameter || flag_status == osFlagsErrorResource) {
        return false;
    }
    modem_api_flag_lut[flag].state = state;
    return true;
}
bool Modem_API_ClearFlag (eModemFlagsEnum_t flag, eModemApiStateEnum_t state) {
    uint32_t flag_status = osFlagsErrorUnknown;
    flag_status = osEventFlagsClear(modem_api_flag_lut[eModemFlag_MsgRecieved].event_flag_id, modem_api_flag_lut[flag].event_flag_mask);
    if (flag_status == osFlagsErrorUnknown || flag_status == osFlagsErrorParameter || flag_status == osFlagsErrorResource) {
        return false;
    }
    modem_api_flag_lut[flag].state = state;
    return true;
}
bool Modem_API_WaitFlag (eModemFlagsEnum_t flag, uint32_t timeout) {
    uint32_t flag_status = osFlagsErrorUnknown;
    flag_status = osEventFlagsWait(modem_api_flag_lut[flag].event_flag_id, modem_api_flag_lut[flag].event_flag_mask, osFlagsNoClear, timeout);
    /* ASSERT */
    if (flag_status == osFlagsErrorUnknown || flag_status == osFlagsErrorParameter || flag_status == osFlagsErrorResource) {
        return false;
    }
    /* TIMEOUT */
    if (flag_status == osFlagsErrorTimeout) {
        return false;
    }
    return true;
}
bool SMS_APP_SendAndSetFlag (char *command, eModemFlagsEnum_t flag, eModemApiStateEnum_t state, uint32_t timeout) {
    Modem_API_SendAndWait(command, timeout, RETRY_COUNT);
    if (Modem_API_GetState(eModemFlag_MsgRecieved) == eModemApiState_OK) {
        return Modem_API_SetFlag(flag, state);
    }
    return false;
}
eModemApiStateEnum_t Modem_API_GetState (eModemFlagsEnum_t flag) {
    return modem_api_flag_lut[flag].state;
}
// ONE FUNCTION THAT READS SMS FOR SAY 100MS
// RESET TIMOUT AFTER EVERY MESSAGE
// SEPARATE FUN FOR SMS
// SMS DATA - NUM/DATE/
