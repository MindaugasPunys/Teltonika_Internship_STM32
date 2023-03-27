/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "ntp_api.h"
#include "modem_api.h"

#include "string_utils.h"
#include "heap_api.h"

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define NTP_STRUCT_SIZE 48 //BYTES
#define NTP_TIMESTAMP_DELTA 2208988800ull
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
sNtpApi_NtpStruct_t ntp_packet = {0};
/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/
static bool NTP_API_StructToHexStr (sNtpApi_NtpStruct_t *ntp_struct, char **ntp_hex_string);
static bool NTP_API_HexStrToStruct (char *ntp_hex_string, sNtpApi_NtpStruct_t *ntp_struct);

static uint8_t* NTP_API_StructToArray (sNtpApi_NtpStruct_t *ntp_struct);
static bool NTP_API_ArrayToStruct (uint8_t *ntp_array, sNtpApi_NtpStruct_t *ntp_struct);
static bool NTP_API_AppendUint32 (uint32_t element, uint8_t *ntp_array, uint16_t *ntp_index); // TO UTILITIES!
/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
static bool NTP_API_StructToHexStr (sNtpApi_NtpStruct_t *ntp_struct, char **ntp_hex_string) {
    /* Uses dynamically allocated memory. Use Heap_Api_Free(ntp_hex_string). */
    if (ntp_struct == NULL) {
        return false;
    }

    /* STRUCT TO ARRAY */
    uint8_t *ntp_array = NTP_API_StructToArray(ntp_struct);
    StrToHex((char*) ntp_array, NTP_STRUCT_SIZE, ntp_hex_string);
    Heap_API_Free(ntp_array);

    return true;
}
static bool NTP_API_HexStrToStruct (char *ntp_hex_string, sNtpApi_NtpStruct_t *ntp_struct) {
    if (ntp_hex_string == NULL) {
        return false;
    }
    StrRemChar(ntp_hex_string, "\"");
    char *ntp_string = NULL;
    StrHexToStr(ntp_hex_string, NTP_STRUCT_SIZE * 2, &ntp_string);
    NTP_API_ArrayToStruct((uint8_t*) ntp_string, ntp_struct);
    Heap_API_Free(ntp_string);
    return true;
}

static uint8_t* NTP_API_StructToArray (sNtpApi_NtpStruct_t *ntp_struct) {
    uint8_t *ntp_array = Heap_API_Allocate(NTP_STRUCT_SIZE + 1, sizeof(uint8_t));
    ntp_array[0] = ntp_struct->li_vn_mode;
    ntp_array[1] = ntp_struct->stratum;
    ntp_array[2] = ntp_struct->poll;
    ntp_array[3] = ntp_struct->precision;

    uint16_t ntp_index = 4;
    NTP_API_AppendUint32(ntp_struct->rootDelay, ntp_array, &ntp_index);
    NTP_API_AppendUint32(ntp_struct->rootDispersion, ntp_array, &ntp_index);
    NTP_API_AppendUint32(ntp_struct->refId, ntp_array, &ntp_index);

    NTP_API_AppendUint32(ntp_struct->refTm_s, ntp_array, &ntp_index);
    NTP_API_AppendUint32(ntp_struct->refTm_f, ntp_array, &ntp_index);

    NTP_API_AppendUint32(ntp_struct->origTm_s, ntp_array, &ntp_index);
    NTP_API_AppendUint32(ntp_struct->origTm_f, ntp_array, &ntp_index);

    NTP_API_AppendUint32(ntp_struct->rxTm_s, ntp_array, &ntp_index);
    NTP_API_AppendUint32(ntp_struct->rxTm_f, ntp_array, &ntp_index);

    NTP_API_AppendUint32(ntp_struct->txTm_s, ntp_array, &ntp_index);
    NTP_API_AppendUint32(ntp_struct->txTm_f, ntp_array, &ntp_index);

    return ntp_array;
}
static bool NTP_API_ArrayToStruct (uint8_t *ntp_array, sNtpApi_NtpStruct_t *ntp_struct) {
    ntp_struct->li_vn_mode = ntp_array[0];
    ntp_struct->stratum = ntp_array[1];
    ntp_struct->poll = ntp_array[2];
    ntp_struct->precision = ntp_array[3];

    StrUint8ToUint32(&ntp_array[4], &ntp_struct->rootDelay, false);
    StrUint8ToUint32(&ntp_array[8], &ntp_struct->rootDispersion, false);
    StrUint8ToUint32(&ntp_array[12], &ntp_struct->refId, false);

    StrUint8ToUint32(&ntp_array[16], &ntp_struct->refTm_s, false);
    StrUint8ToUint32(&ntp_array[20], &ntp_struct->refTm_f, false);

    StrUint8ToUint32(&ntp_array[24], &ntp_struct->origTm_s, false);
    StrUint8ToUint32(&ntp_array[28], &ntp_struct->origTm_f, false);

    StrUint8ToUint32(&ntp_array[32], &ntp_struct->rxTm_s, false);
    StrUint8ToUint32(&ntp_array[36], &ntp_struct->rxTm_f, false);

    StrUint8ToUint32(&ntp_array[40], &ntp_struct->txTm_s, false);
    StrUint8ToUint32(&ntp_array[44], &ntp_struct->txTm_f, false);

    return true;
}
static bool NTP_API_AppendUint32 (uint32_t element, uint8_t *ntp_array, uint16_t *ntp_index) {
    uint8_t u8_buffer[4] = {0};
    StrUint32ToUint8(element, u8_buffer, false);
    for (uint8_t i = 0; i < 4; i++) {
        ntp_array[*ntp_index] = u8_buffer[i];
        *ntp_index += 1;
    }
    return true;
}

/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool NTP_API_GetNtp (time_t *utc_time, sNetworkApp_Socket_t *socket) {
    /* PREP MESSAGE STRUCT */
    memset(&ntp_packet, 0, sizeof(sNtpApi_NtpStruct_t));
    ntp_packet.li_vn_mode = 0b00011011; // li = 0, vn = 3, and mode = 3

    /* PACKET TO HEX */
    char *hex_buffer = NULL;
    NTP_API_StructToHexStr(&ntp_packet, &hex_buffer);

    /* SEND */
    Network_APP_OpenSocket(socket);
    char *hex_message = Heap_API_Allocate(strlen(hex_buffer) + CMD_STRING_LENGTH, sizeof(char));
    sprintf(hex_message, "AT+EIPSEND=%d,\"%s\"\n", socket->id, hex_buffer);
    Modem_API_SendAndWait(hex_message, DEFAULT_WAIT_TIME, 2);

    /* FINISH SEND */
    Heap_API_Free(hex_buffer);
    Heap_API_Free(hex_message);

    /* GET NTP BUFFER*/
    char *ntp_buffer = NULL;
    if (Modem_API_WaitFlag(eModemFlag_UdpRecieved, DEFAULT_WAIT_TIME) == true) {
        Modem_API_ClearFlag(eModemFlag_UdpRecieved, eModemApiState_NotReady);
        Network_APP_ReadSocket();

        if (Modem_API_WaitFlag(eModemFlag_UdpRecieved, DEFAULT_WAIT_TIME) == true) {
            Modem_API_ClearFlag(eModemFlag_UdpRecieved, eModemApiState_NotReady);
            ntp_buffer = Heap_API_Allocate(NTP_STRUCT_SIZE * 2 + 4, sizeof(char));
            Modem_API_GetResponse(eModemApiCmd_EIPRECV, ntp_buffer);
        }
    }

    /* NTP HEX TO PACKET */
    if (ntp_buffer == NULL) {
        return false;
    }
    memset(&ntp_packet, 0, sizeof(sNtpApi_NtpStruct_t));
    NTP_API_HexStrToStruct(ntp_buffer, &ntp_packet);
    *utc_time = (time_t) (ntp_packet.txTm_s - NTP_TIMESTAMP_DELTA);
//    debug("%s\n", ctime((const time_t*) &current_utc));
//    Network_APP_CloseUdp();
    Heap_API_Free(ntp_buffer);
    return true;
}

