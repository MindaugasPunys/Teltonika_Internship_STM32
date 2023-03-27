#ifndef __MODEM_API__H__
#define __MODEM_API__H__

/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "stdbool.h"
#include "stdint.h"
/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/
#define SECOND_IN_MS 1000
#define DEFAULT_WAIT_TIME 5*SECOND_IN_MS
/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/
typedef enum eModemApiCommandEnum_t {
    eModemApiCmd_First = 0,
        eModemApiCmd_OK = eModemApiCmd_First,
        eModemApiCmd_ERROR,

        eModemApiCmd_EIND,
        eModemApiCmd_EUSIM,
        eModemApiCmd_ECHO,

        eModemApiCmd_MsgSent,
        eModemApiCmd_MsgRef,
        eModemApiCmd_CPMS,
        eModemApiCmd_CMTI,
        eModemApiCmd_CMGL,
        eModemApiCmd_CMGR,

        eModemApiCmd_Start,
        eModemApiCmd_Stop,

        eModemApiCmd_CGDCONT,
        eModemApiCmd_EGDCONT,
        eModemApiCmd_CGATT,
        eModemApiCmd_CGACT,
        eModemApiCmd_CGPADDR,

        eModemApiCmd_ETL,
        eModemApiCmd_EIPSEND,
        eModemApiCmd_ESOCK,
        eModemApiCmd_EIPRECV,

        eModemApiCmd_Last
} eModemApiCommandEnum_t;

typedef enum eModemFlagsEnum_t {
    eModemFlag_First = 0,

    eModemFlag_Ready = eModemFlag_First,
        eModemFlag_MsgRecieved,
        eModemFlag_Echo,
        eModemFlag_Setup,

        eModemFlag_Sim1,
        eModemFlag_Sim2,
        eModemFlag_TextMode,
        eModemFlag_SmsStore,
        eModemFlag_SmsRead,
        eModemFlag_SmsRecieved,

        eModemFlag_PdpContext,
        eModemFlag_PdpAttach,
        eModemFlag_PdpActive,
        eModemFlag_TcpContext,
        eModemFlag_TcpSocket,
        eModemFlag_TcpRecieved,
        eModemFlag_UdpSocket,
        eModemFlag_UdpRecieved,

        eModemFlag_Last,
} eModemFlagsEnum_t;
typedef enum eModemApiStateEnum_t {
    eModemApiState_First = 0,
        eModemApiState_Unknown = eModemApiState_First,
        eModemApiState_Ready,
        eModemApiState_NotReady,
        eModemApiState_OK,
        eModemApiState_ERROR,
        eModemApiState_ON,
        eModemApiState_OFF,
        eModemApiState_Last,
} eModemApiStateEnum_t;

/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
bool Modem_API_Init (void);

bool Modem_API_PutResponse (eModemApiCommandEnum_t response_enum, char *response);
bool Modem_API_GetResponse (eModemApiCommandEnum_t response_enum, char *response);

bool Modem_API_SendAndWait (char *command_buffer, uint32_t timeout, uint32_t try_count);
bool Modem_API_Send (char *command_buffer);

bool Modem_API_SetFlag (eModemFlagsEnum_t flag, eModemApiStateEnum_t state);
bool Modem_API_ClearFlag (eModemFlagsEnum_t flag, eModemApiStateEnum_t state);
bool Modem_API_WaitFlag (eModemFlagsEnum_t flag, uint32_t timeout);
bool SMS_APP_SendAndSetFlag (char *command, eModemFlagsEnum_t flag, eModemApiStateEnum_t state, uint32_t timout);
eModemApiStateEnum_t Modem_API_GetState (eModemFlagsEnum_t flag);
#endif /* __SOMETHING__H__ */
