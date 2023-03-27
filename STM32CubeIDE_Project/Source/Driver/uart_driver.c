/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "stm32f4xx_ll_usart.h"
#include "string.h"

#include "gpio_driver.h"
#include "circ_buffer.h"
#include "uart_driver.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define BUFFER_OVERHEAD 3
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
typedef struct sUartDriver_Static_t {
    USART_TypeDef *usart;
    eGpio_Enum_t rx;
    eGpio_Enum_t tx;

    /* Modem GPIO */
    bool hw_control_enabled;
    eGpio_Enum_t dtr;
    eGpio_Enum_t cts;
    eGpio_Enum_t rts;

    /* UART configuration */
    uint32_t data_width;
    uint32_t stop_bits;
    uint32_t parity;
    uint32_t transfer_direction;
    uint32_t hardware_flow_control;
    uint32_t over_sampling;

    /* Functions */
    void (*clock_function) (uint32_t);
    uint32_t clock_function_args;
    uint16_t interupt;
} sUartDriver_Static_t;

typedef struct sUartDriver_Dynamic_t {
    cbuf_handle_t circ_buffer_ptr;
} sUartDriver_Dynamic_t;

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
const static sUartDriver_Static_t static_uart_lut[eUartDriver_Last] = {
    [eUartDriver_USART1] = {
        .usart = USART1,
        .rx = eGpioDriver_UartUsart1RXPin,
        .tx = eGpioDriver_UartUsart1TXPin,
        .hw_control_enabled = false,
        .dtr = eGpioDriver_Last,
        .cts = eGpioDriver_Last,
        .rts = eGpioDriver_Last,
        .data_width = LL_USART_DATAWIDTH_8B,
        .stop_bits = LL_USART_STOPBITS_1,
        .parity = LL_USART_PARITY_NONE,
        .transfer_direction = LL_USART_DIRECTION_TX_RX,
        .hardware_flow_control = LL_USART_HWCONTROL_NONE,
        .over_sampling = LL_USART_OVERSAMPLING_16,
        .clock_function = LL_APB2_GRP1_EnableClock,
        .clock_function_args = LL_APB2_GRP1_PERIPH_USART1,
        .interupt = USART1_IRQn},

    [eUartDriver_USART2] = {
        .usart = USART2,
        .rx = eGpioDriver_UartUsart2RXPin,
        .tx = eGpioDriver_UartUsart2TXPin,
        .hw_control_enabled = true,
        .dtr = eGpioDriver_ModemUARTDTRPin,
        .cts = eGpioDriver_ModemUARTCTSPin,
        .rts = eGpioDriver_ModemUARTRTSPin,
        .data_width = LL_USART_DATAWIDTH_8B,
        .stop_bits = LL_USART_STOPBITS_1,
        .parity = LL_USART_PARITY_NONE,
        .transfer_direction = LL_USART_DIRECTION_TX_RX,
        .hardware_flow_control = LL_USART_HWCONTROL_NONE,
        .over_sampling = LL_USART_OVERSAMPLING_16,
        .clock_function = LL_APB1_GRP1_EnableClock,
        .clock_function_args = LL_APB1_GRP1_PERIPH_USART2,
        .interupt = USART2_IRQn
    }
};
/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
sUartDriver_Dynamic_t dynamic_uart_lut[eUartDriver_Last];

/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/
static void UART_Driver_RecieveCallback (eUart_Driver_Enum_t uart);
/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
static void UART_Driver_RecieveCallback (eUart_Driver_Enum_t uart) {
    if (LL_USART_IsEnabledIT_RXNE(static_uart_lut[uart].usart) && LL_USART_IsActiveFlag_RXNE(static_uart_lut[uart].usart)) {
        uint8_t byte = 0;
        byte = LL_USART_ReceiveData8(static_uart_lut[uart].usart);
        CIRC_Buffer_put(dynamic_uart_lut[uart].circ_buffer_ptr, byte);
    }
}

void USART1_IRQHandler (void) {
    UART_Driver_RecieveCallback(eUartDriver_USART1);
}

void USART2_IRQHandler (void) {
    UART_Driver_RecieveCallback(eUartDriver_USART2);
}

static bool UART_Driver_InitGpio (eUart_Driver_Enum_t uart) {
    /* Enable USART GPIO RX TX pins */
    if (uart >= eUartDriver_Last) {
        return false;
    }

    if (GPIO_Driver_Init(static_uart_lut[uart].rx) == false) {
        return false;
    }
    if (GPIO_Driver_Init(static_uart_lut[uart].tx) == false) {
        return false;
    }

    if (static_uart_lut[uart].hw_control_enabled == true) {
        if (GPIO_Driver_Init(static_uart_lut[uart].cts) == false) {
            return false;
        }
        if (GPIO_Driver_Init(static_uart_lut[uart].rts) == false) {
            return false;
        }
        if (GPIO_Driver_Init(static_uart_lut[uart].dtr) == false) {
            return false;
        }
    }

    return true;
}

/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/

bool UART_Driver_Init (eUart_Driver_Enum_t uart, uint32_t baudRate, uint16_t message_size) {
    /* Initialize GPIO */
    if (UART_Driver_InitGpio(uart) == false) {
        return false;
    }

    /* Peripheral clock enable */
    static_uart_lut[uart].clock_function(static_uart_lut[uart].clock_function_args);

    /* USART interrupt Init */
    NVIC_SetPriority(static_uart_lut[uart].interupt, 32);
    NVIC_EnableIRQ(static_uart_lut[uart].interupt);

    /* Initialize USART configuration */
    LL_USART_InitTypeDef USART_InitStruct = {0};
    USART_InitStruct.BaudRate = baudRate;
    USART_InitStruct.DataWidth = static_uart_lut[uart].data_width;
    USART_InitStruct.StopBits = static_uart_lut[uart].stop_bits;
    USART_InitStruct.Parity = static_uart_lut[uart].parity;
    USART_InitStruct.TransferDirection = static_uart_lut[uart].transfer_direction;
    USART_InitStruct.HardwareFlowControl = static_uart_lut[uart].hardware_flow_control;
    USART_InitStruct.OverSampling = static_uart_lut[uart].over_sampling;

    ErrorStatus status = ERROR;
    status = LL_USART_Init(static_uart_lut[uart].usart, &USART_InitStruct);
    if (status != SUCCESS) {
        return false;
    }
    LL_USART_ConfigAsyncMode(static_uart_lut[uart].usart);
    LL_USART_Enable(static_uart_lut[uart].usart);

    /* Enable RXNE interrupt */
    LL_USART_EnableIT_RXNE(static_uart_lut[uart].usart);

    /* Allocate a circular buffer */
    dynamic_uart_lut[uart].circ_buffer_ptr = CIRC_Buffer_init(message_size * BUFFER_OVERHEAD);
    if (dynamic_uart_lut[uart].circ_buffer_ptr == NULL) {
        return false;
    }
    return true;
}

bool UART_Driver_SendString (eUart_Driver_Enum_t uart, char *data, uint16_t length) {
    if (uart >= eUartDriver_Last) {
        return false;
    }
    if (data == NULL) {
        return false;
    }
    if (length == 0) {
        return false;
    }

    for (uint16_t i = 0; i < length; i++) {
        while (LL_USART_IsActiveFlag_TXE(static_uart_lut[uart].usart) == 0) {
        }
        LL_USART_TransmitData8(static_uart_lut[uart].usart, (uint8_t) data[i]);
    }
    return true;
}

bool UART_Driver_GetByte (eUart_Driver_Enum_t uart, uint8_t *dest) {
    if (uart >= eUartDriver_Last) {
        return false;
    }
    if (dest == NULL) {
        return false;
    }

    return CIRC_Buffer_get(dynamic_uart_lut[uart].circ_buffer_ptr, dest);
}
