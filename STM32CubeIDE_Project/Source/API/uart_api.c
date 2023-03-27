/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "uart_api.h"
#include "uart_driver.h"
#include "heap_api.h"
#include "debug_api.h"
#include "message.h"
#include "string_utils.h"

#include "stdlib.h"
#include "main.h"
#include "string.h"
#include "cmsis_os2.h"

/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define MESSAGE_QUEUE_NUM 4
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
typedef enum eUartEnumState_t {
    eUartState_First = 0,
        eUartState_Setup = eUartState_First,
        eUartState_Collect,
        eUartState_Flush,
        eUartState_Last
} eUartEnumState_t;

typedef struct sUartApiStatic_t {
    uint16_t buffer_length;
} sUartApiStatic_t;

typedef struct sUartApiDynamic_t {
    uint16_t buffer_index;
    char *buffer_ptr;
    eUartEnumState_t state;
    eUartBaudrate_t baud_rate;
    eUart_Driver_Enum_t driver;
    osMutexId_t mutex_id;
    osMessageQueueId_t message_queue;
    char *delim;
} sUartApiDynamic_t;

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
static const sUartApiStatic_t static_uart_api_lut[eUartDriver_Last] = {
    [eUart_Debug] = {.buffer_length = 64},
    [eUart_Modem] = {.buffer_length = 1024}
};

INIT_DEBUG_MODULE(UART_API);
static const osThreadAttr_t uart_api_task_attributes = {.name = "UART API task", .priority = (osPriority_t) osPriorityBelowNormal, .stack_size = 1024};
static const osMutexAttr_t mutex_attributes = {"uart_api_mutex", 0, NULL, 0U};

/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
static sUartApiDynamic_t dynamic_uart_api_lut[eUartDriver_Last] = {
    [eUart_Debug] = {.state = eUartState_First, .baud_rate = eUartBaudrate_Default, .driver = eUartDriver_USART1, .mutex_id = NULL, .message_queue = NULL, .delim = "\r\n"},
    [eUart_Modem] = {.state = eUartState_First, .baud_rate = eUartBaudrate_Default, .driver = eUartDriver_USART2, .mutex_id = NULL, .message_queue = NULL, .delim = "\r\n"},
};
static osThreadId_t uart_api_task_handle;

static char recieved_byte = 0;
/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
static void UART_API_Thread (void *argument) {
    while (true) {
        for (eUart_Driver_Enum_t uart = eUartDriver_First; uart < eUartDriver_Last; uart++) {
            /* STATE MACHINE FOR EACH UART */
            switch (dynamic_uart_api_lut[uart].state) {

                case eUartState_Setup: {
                    dynamic_uart_api_lut[uart].buffer_index = 0;
                    dynamic_uart_api_lut[uart].buffer_ptr = Heap_API_Allocate(static_uart_api_lut[uart].buffer_length, sizeof(char));
                    if (dynamic_uart_api_lut[uart].buffer_ptr == NULL) {
                        error("Failed to initialize buffer\n");
                        break;
                    }
                    dynamic_uart_api_lut[uart].state = eUartState_Collect;
                    break;
                }

                case eUartState_Collect:
                    {
                    recieved_byte = 0;
                    while (UART_Driver_GetByte(uart, (uint8_t*) &recieved_byte) == true) {
                        if (recieved_byte == 0) {
                            error("Received byte 0. (USART%u)\n", uart + 1);
                            break;
                        }
                        if (StrContains(&recieved_byte, 1, dynamic_uart_api_lut[uart].delim)) {
                            dynamic_uart_api_lut[uart].buffer_ptr[dynamic_uart_api_lut[uart].buffer_index] = '\n';
                            dynamic_uart_api_lut[uart].buffer_index++;
                            dynamic_uart_api_lut[uart].state = eUartState_Flush;
                            break;
                        }
                        dynamic_uart_api_lut[uart].buffer_ptr[dynamic_uart_api_lut[uart].buffer_index] = recieved_byte;
                        dynamic_uart_api_lut[uart].buffer_index++;

                        if (dynamic_uart_api_lut[uart].buffer_index == static_uart_api_lut[uart].buffer_length) {
                            error("Message buffer overflow. (USART%u)\n", uart + 1);
                            dynamic_uart_api_lut[uart].state = eUartState_Setup;
                            Heap_API_Free(dynamic_uart_api_lut[uart].buffer_ptr);
                            dynamic_uart_api_lut[uart].buffer_ptr = NULL;
                            break;
                        }
                    }
                    break;
                }

                case eUartState_Flush:
                    {
                    /* Add new message to queue */
                    sMessage_t message = {
                        .buffer = dynamic_uart_api_lut[uart].buffer_ptr,
                        .length = dynamic_uart_api_lut[uart].buffer_index
                    };
                    if (osMessageQueuePut(dynamic_uart_api_lut[uart].message_queue, &message, 0U, 0U) != osOK) {
                        error("Failed to put message in message queue\n");
                        Heap_API_Free(message.buffer);
                    }
                    dynamic_uart_api_lut[uart].state = eUartState_Setup;
                    break;
                }

                default: {
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
bool UART_API_Init (eUartAPIEnum_t uart, char *delim, eUartBaudrate_t baud_rate) {
    if (uart >= eUart_Last) {
        return false;
    }
    /* INIT UART */
    if (delim != NULL) {
        dynamic_uart_api_lut[uart].delim = delim;
    }
    if (baud_rate < eUartBaudrate_Last) {
        dynamic_uart_api_lut[uart].baud_rate = baud_rate;
    }
    UART_Driver_Init(dynamic_uart_api_lut[uart].driver, dynamic_uart_api_lut[uart].baud_rate, static_uart_api_lut[uart].buffer_length);

    /* MUTEX */
    if (dynamic_uart_api_lut[uart].mutex_id == NULL) {
        dynamic_uart_api_lut[uart].mutex_id = osMutexNew(&mutex_attributes);
        if (dynamic_uart_api_lut[uart].mutex_id == NULL) {
            error("Failed to create MUTEX\n");
            return false;
        }
    }

    /* MESSAGE QUEUE */
    if (dynamic_uart_api_lut[uart].message_queue == NULL) {
        dynamic_uart_api_lut[uart].message_queue = osMessageQueueNew(MESSAGE_QUEUE_NUM, sizeof(sMessage_t), NULL);
        if (dynamic_uart_api_lut[uart].message_queue == NULL) {
            error("Failed to create message queue\n");
            osMutexDelete(dynamic_uart_api_lut[uart].mutex_id);
            return false;
        }
    }

    /* THREAD */
    if (uart_api_task_handle == NULL) {
        uart_api_task_handle = osThreadNew(UART_API_Thread, NULL, &uart_api_task_attributes);
        if (uart_api_task_handle == NULL) {
            osMessageQueueDelete(dynamic_uart_api_lut[uart].message_queue);
            osMutexDelete(dynamic_uart_api_lut[uart].mutex_id);
            return false;
        }

    }
    return true;
}

//bool UART_API_PutMessage (sMessage_t *message, eUartAPIEnum_t uart) {
//    if (message == NULL) {
//        error("Argument message == NULL\n");
//        return false;
//    }
//    if (osMessageQueuePut(dynamic_uart_api_lut[uart].message_queue, &message, 0U, 0U) != osOK) {
//        error("Failed to put message in message queue\n");
//        Heap_API_Free(message->buffer);
//    }
//    return true;
//}

bool UART_API_GetMessage (sMessage_t *message, eUartAPIEnum_t uart) {
    if (message == NULL) {
        error("Argument message == NULL\n");
        return false;
    }
    if (osMessageQueueGet(dynamic_uart_api_lut[uart].message_queue, message, 0U, osWaitForever) != osOK) {
        error("Failed to get message from message queue\n");
        return false;
    }
    return true;
}

bool UART_API_SendString (eUartAPIEnum_t uart, char *data, uint16_t length) {
    if (osMutexAcquire(dynamic_uart_api_lut[uart].mutex_id, osWaitForever) != osOK) {
        return false;
    }
    bool status = false;
    status = UART_Driver_SendString(uart, data, length);
    if (osMutexRelease(dynamic_uart_api_lut[uart].mutex_id) != osOK) {
        return false;
    }
    return status;
}

