#ifndef __UART_API__H__
#define __UART_API__H__

/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "cmsis_os2.h"
#include "stdbool.h"
#include "message.h"

/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/
#define UART_API_Send(uart, data) UART_API_SendString (uart, data, strlen(data))

typedef enum eUartAPIEnum_t {
eUart_First,
    eUart_Debug = eUart_First,
    eUart_Modem,
    eUart_Last
} eUartAPIEnum_t;

typedef enum eUartBaudrate_t {
eUartBaudrate_9600 = 9600,
    eUartBaudrate_19200 = 19200,
    eUartBaudrate_28800 = 28800,
    eUartBaudrate_38400 = 38400,
    eUartBaudrate_57600 = 57600,
    eUartBaudrate_76800 = 76800,
    eUartBaudrate_115200 = 115200,
    eUartBaudrate_230400 = 230400,
    eUartBaudrate_460800 = 460800,
    eUartBaudrate_576000 = 576000,
    eUartBaudrate_921600 = 921600,
    eUartBaudrate_Last,
    eUartBaudrate_Default = eUartBaudrate_115200
} eUartBaudrate_t;
/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
bool UART_API_Init (eUartAPIEnum_t uart, char *delim, eUartBaudrate_t baud_rate);
bool UART_API_GetMessage (sMessage_t *message, eUartAPIEnum_t uart);
bool UART_API_SendString (eUartAPIEnum_t uart, char *data, uint16_t length);

#endif /* __UART_API__H__ */
