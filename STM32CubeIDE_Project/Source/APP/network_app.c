/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "network_app.h"
#include "sms_app.h"
#include "modem_api.h"
#include "ntp_api.h"

#include "string_utils.h"
#include "heap_api.h"
#include "debug_api.h"
#include "rtc_driver.h"

#include "cmsis_os2.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/

INIT_DEBUG_MODULE(NETWORK)

#define DEST_IP "88.119.190.245"
#define DEST_PORT 10034
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
static const osThreadAttr_t network_task_attributes = {.name = "NetworkApp", .priority = (osPriority_t) osPriorityBelowNormal, .stack_size = 1024};
/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
static osThreadId_t tcp_app_task_handle;

/* Sockets */
static sNetworkApp_Socket_t tcp_socket = {.type = eSocket_Tcp, .open = false, .id = 0, .ip = "88.119.190.245", .port = 10034};
static sNetworkApp_Socket_t udp_socket = {.type = eSocket_Udp, .open = false, .id = 0, .ip = "88.119.190.245", .port = 10034};
static sNetworkApp_Socket_t ntp_socket = {.type = eSocket_Ntp, .open = false, .id = 0, .ip = "193.219.61.120", .port = 123}; // KTU NTP

static bool send_tcp_message = false;

static char command_buffer[CMD_STRING_LENGTH] = {0};
static char tcp_message[SMS_LENGTH] = {0};

time_t current_utc = 0;
/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/

static void Network_APP_Thread (void *argument) {
    Modem_API_WaitFlag(eModemFlag_Setup, osWaitForever);

    while (true) {
        /* TCP-IP */
        if (send_tcp_message == true) {
            Network_APP_SendTcp(tcp_message);
            Modem_API_GetResponse(eModemApiCmd_OK, command_buffer);

            if (strcmp(command_buffer, "ERROR") == 0) {
                Network_APP_CloseSocket(&tcp_socket);
                send_tcp_message = false;
            }
            osDelay(10 * SECOND_IN_MS);
        } else {
            RTC_Driver_GetTime();

            /* NTP */
            NTP_API_GetNtp(&current_utc, &ntp_socket);
            debug("%s", ctime((const time_t* ) &current_utc));

            /* UDP */
            Network_APP_SendUdp("Hello world!\n");
            osDelay(30 * SECOND_IN_MS);
        }
    }
}
/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool Network_APP_Init (void) {
    /* THREAD */
    tcp_app_task_handle = osThreadNew(Network_APP_Thread, NULL, &network_task_attributes);
    if (tcp_app_task_handle == NULL) {
        return false;
    }
    return true;
}

bool Network_APP_SetState (bool state, char *ip, uint16_t port, char *message) {
    /* ASSERT */
    if (state == true && (ip == NULL || port == 0)) {
        return false;
    }
    bool previous_state = send_tcp_message;
    send_tcp_message = false; // Don't send while changing state!

    /* COPY MESSAGE */
    if (state == true) {
        tcp_socket.port = port;
        strncpy(tcp_socket.ip, ip, IP_LENGTH);
        snprintf(tcp_message, SMS_LENGTH, "%s\n", message);
    }

    /* OPEN/CLOSE SOCKET */
    if (state == true && previous_state == false) {
        Network_APP_OpenSocket(&tcp_socket);
    }
    if (state == false && previous_state == true) {
        Network_APP_CloseSocket(&tcp_socket);
    }
    send_tcp_message = state;
    return true;
}

/* SOCKET COMMANDS */
bool Network_APP_OpenSocket (sNetworkApp_Socket_t *socket) {
    Network_APP_AttachMT();
    /* CREATE SOCKET */
    if (socket->open == false) {
        // REMOVE HARDCODE
        // SERVER LUT
        if (socket->type == eSocket_Tcp) {
            sprintf(command_buffer, "AT+ETL=1,%d,0,\"%s\",%d\n", DATA_ACC_ID, socket->ip, socket->port);
        } else {
            sprintf(command_buffer, "AT+ETL=1,%d,1,\"%s\",%d\n", DATA_ACC_ID, socket->ip, socket->port);
        }

        socket->open = Modem_API_SendAndWait(command_buffer, 60 * SECOND_IN_MS, 5);
        if (socket->open == false) {
            Network_APP_DetachMT();
        }
        /* GET SOCKET ID */
        Modem_API_GetResponse(eModemApiCmd_ETL, command_buffer);
        socket->id = atol(command_buffer);
    }
    return socket->open;
}
bool Network_APP_CloseSocket (sNetworkApp_Socket_t *socket) {
    /* CLOSE SOCKET */
    if (socket->open == true) {
        sprintf(command_buffer, "AT+ETL=0,%d\n", socket->id);
        socket->open = !Modem_API_SendAndWait(command_buffer, 20 * SECOND_IN_MS, 5);
    }
    Network_APP_DetachMT();
    return !socket->open;
}
bool Network_APP_ReadSocket (void) {
    char id[3] = {0};
    Modem_API_GetResponse(eModemApiCmd_ESOCK, id);
    sprintf(command_buffer, "AT+EIPRECV=%s\n", id);
    Modem_API_SendAndWait(command_buffer, SECOND_IN_MS, 1);
    return true;
}
bool Network_APP_AttachMT (void) {
    if (Modem_API_GetState(eModemFlag_PdpAttach) != eModemApiState_Ready) {
        sprintf(command_buffer, "AT+CGATT=1\n");
        if (SMS_APP_SendAndSetFlag(command_buffer, eModemFlag_PdpAttach, eModemApiState_Ready, DEFAULT_WAIT_TIME) == false) {
            return false;
        }
    }
    return true;
}
bool Network_APP_DetachMT (void) {
    if (tcp_socket.open == false || udp_socket.open == false || ntp_socket.open == false) {
        return false;
    }

    if (Modem_API_GetState(eModemFlag_PdpAttach) == eModemApiState_Ready) {
        sprintf(command_buffer, "AT+CGATT=0\n");
        if (SMS_APP_SendAndSetFlag(command_buffer, eModemFlag_PdpAttach, eModemApiState_NotReady, DEFAULT_WAIT_TIME) == false) {
            return false;
        }
    }
    return true;
}

bool Network_APP_SendTcp (char *message) {
    /* ASSERT */
    if (message == NULL) {
        return false;
    }

    /* SEND */
    char *hex_buffer = NULL;
    if (StrToHex(message, 0, &hex_buffer) == false) {
        return false;
    }
    char *hex_message = Heap_API_Allocate(strlen(hex_buffer) + CMD_STRING_LENGTH, sizeof(char));
    sprintf(hex_message, "AT+EIPSEND=%d,\"%s\"\n", tcp_socket.id, hex_buffer);
    Modem_API_SendAndWait(hex_message, DEFAULT_WAIT_TIME, 5);

    /* FINISH */
    Heap_API_Free(hex_message);
    Heap_API_Free(hex_buffer);

    return true;
}

bool Network_APP_SendUdp (char *message) {
    Network_APP_OpenSocket(&udp_socket);
    /* SEND */
    char *hex_buffer = NULL;
    if (StrToHex(message, 0, &hex_buffer) == false) {
        return false;
    }
    char *hex_message = Heap_API_Allocate(strlen(hex_buffer) + CMD_STRING_LENGTH, sizeof(char));
    sprintf(hex_message, "AT+EIPSEND=%d,\"%s\"\n", udp_socket.id, hex_buffer);
    Modem_API_SendAndWait(hex_message, DEFAULT_WAIT_TIME, 5);
    /* FINISH */
    Heap_API_Free(hex_message);
    Heap_API_Free(hex_buffer);

    Network_APP_CloseSocket(&udp_socket);
    return true;
}

// TCP/UDP/NTP sockets
// GET/SET - TCP/UDP/NTP sockets

// Ring buffer su ip
// GPRS
