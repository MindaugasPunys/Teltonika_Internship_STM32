#ifndef __TCP_APP__H__
#define __TCP_APP__H__

/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "stdbool.h"
#include "stdint.h"
/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/
#define IP_LENGTH 32
#define CMD_STRING_LENGTH 64
/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/
typedef enum eSocketType_t {
    eSocket_First = 0,
        eSocket_Tcp = eSocket_First,
        eSocket_Udp,
        eSocket_Ntp,
        eSocket_Last
} eSocketType_t;

typedef struct sNetworkApp_Socket_t {
    eSocketType_t type;
    bool open;

    uint16_t id;
    char ip[IP_LENGTH];
    uint16_t port;
} sNetworkApp_Socket_t;
/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
bool Network_APP_Init (void);
bool Network_APP_SetState (bool state, char *ip, uint16_t port, char *message);

bool Network_APP_OpenTcp (char *ip, uint16_t port);
bool Network_APP_CloseTcp (void);
bool Network_APP_SendTcp (char *message);

bool Network_APP_OpenUdp (char *ip, uint16_t port);
bool Network_APP_CloseUdp (void);

bool Network_APP_SendUdp (char *message);

bool Network_APP_OpenSocket (sNetworkApp_Socket_t *socket);
bool Network_APP_CloseSocket (sNetworkApp_Socket_t *socket);
bool Network_APP_ReadSocket (void);
bool Network_APP_AttachMT (void);
bool Network_APP_DetachMT (void);
#endif /* __TCP_APP__H__ */
