/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "string_utils.h"
#include "heap_api.h"

#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "stdio.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool StrContains (const char *string, size_t string_size, const char *chars) {
    for (uint16_t i = 0; i < string_size; i++) {
        if (strchr(chars, string[i]) != NULL) {
            return true;
        }
    }
    return false;
}
int StrCountChars (const char *string, size_t string_size, const char *chars) {
    uint16_t chars_size = strlen(chars);
    int count = 0;
    for (uint16_t i = 0; i < string_size; i++) {
        if (string[i] == '\0') {
            break;
        }
        for (uint16_t c = 0; c < chars_size; c++) {
            if (string[i] == chars[c]) {
                count++;
                break;
            }
        }
    }
    return count;
}

bool StrRemChar (char *source, const char *chars) {
    char *char_ptr;
    int index = -1;
    for (int i = 0; i < strlen(chars); i++) {
        char_ptr = strchr(source, chars[i]);
        index = (int) (char_ptr - source);
        while (char_ptr != NULL) {
            memmove(&source[index], &source[index + 1], strlen(source) - index);
            char_ptr = strchr(source, chars[i]);
            index = (int) (char_ptr - source);
        }
    }
    return true;
}
bool StrRemSubstring (char *source, const char *substring) {
    size_t substring_length = strlen(substring);
    if (substring_length > 0) {
        char *substring_ptr = strstr(source, substring);
        if (substring_ptr == NULL) {
            return false;
        }
        size_t remaining_length = strlen(substring_ptr) - substring_length;
        while (substring_ptr != NULL) {
            if (remaining_length > 0) {
                memmove(substring_ptr, substring_ptr + substring_length, strlen(substring_ptr));
            } else {
                strncpy(substring_ptr, "\0", substring_length);
            }
            substring_ptr = strstr(substring_ptr, substring);
            remaining_length = strlen(substring_ptr) - substring_length;
        }
        return true;
    }
    return false;
}
bool StrCompare (const char *string1, const char *string2) {
    if (strlen(string1) == strlen(string2)) {
        if (strcmp(string1, string2) == 0) {
            return true;
        }
    }
    return false;
}
bool StrIsNumber (const char *string) {
    if (string == NULL) {
        return false;
    }
    for (int i = strlen(string) - 1; i != 0; i--) {
        if (isdigit((int) string[i]) != true || string[i] != '.') {
            return false;
        }
    }
    return true;
}
bool StrToUint (const char *string, uint16_t *value) {
    if (string == NULL) {
        return false;
    }
    char *end_ptr;
    *value = strtol(string, &end_ptr, 10);
    if (end_ptr == string) {
        return false;
    }
    return true;
}
bool StrToHex (const char *string, uint16_t string_length, char **hex_string) {
    /* Uses dynamically allocated memory. Use Heap_Api_Free(hex_string). */
    if (string == NULL || hex_string == NULL) {
        return false;
    }
    if (string_length == 0) {
        string_length = strlen(string);
    }
    *hex_string = Heap_API_Allocate(string_length * 2 + 1, sizeof(char));
    for (uint16_t i = 0; i < string_length; i++) {
        sprintf(*hex_string + i * 2, "%02X", string[i]); // write my own code instead of sprintf
    }
    return true;
}
bool StrHexToStr (const char *hex_string, uint16_t hex_string_length, char **string) {
    /* Uses dynamically allocated memory. Use Heap_Api_Free(string). */
    if (string == NULL || hex_string == NULL) {
        return false;
    }
    if (hex_string_length == 0) {
        hex_string_length = strlen(hex_string);
    }
    uint16_t string_length = hex_string_length / 2;

    *string = Heap_API_Allocate(string_length + 1, sizeof(char));
    char buffer[3] = {0};
    for (uint16_t i = 0; i < string_length; i++) {
        strncpy(buffer, hex_string + 2 * i, 2);
        *(*string+i) = (char) strtol(buffer, NULL, 16);
    }
    return true;
}

char* StrLastSubstring (char *string, char *substring) {
    /* NOT USED / TESTED */
    /* Works similar to strstr. Find last occurrence of substring */
    uint16_t string_size = strlen(string);
    uint16_t substring_size = strlen(substring);

    uint16_t match = 0;
    uint16_t sub_i = substring_size - 1;
    for (uint16_t i = string_size - 1; i >= 0; i--) {
        if (string[i] == substring[sub_i]) {
            match++;
            sub_i--;
        } else {
            match = 0;
            sub_i = substring_size - 1;
        }
        if (match == substring_size) {
            return &string[i];
        }
    }
    return false;
}
char* StrLastLine (char *string) {
    uint16_t string_size = strlen(string);
    uint16_t last_line_index = 0;
    for (uint16_t i = 0; i < string_size; i++) {
        if (string[i] == '\n' && string[i + 1] != '\0') {
            last_line_index = i + 1;
        }
    }
    return &string[last_line_index];
}

bool StrUint32ToUint8 (uint32_t u32, uint8_t *u8, bool little_endian) {
    if (u8 == NULL) {
        return false;
    }

    if (little_endian == true) {
        u8[0] = (uint8_t) u32;
        u8[1] = (uint8_t) (u32 >>= 8);
        u8[2] = (uint8_t) (u32 >>= 8);
        u8[3] = (uint8_t) (u32 >>= 8);
    } else {
        u8[3] = (uint8_t) u32;
        u8[2] = (uint8_t) (u32 >>= 8);
        u8[1] = (uint8_t) (u32 >>= 8);
        u8[0] = (uint8_t) (u32 >>= 8);
    }
    return true;
}
bool StrUint8ToUint32 (uint8_t *u8, uint32_t *u32, bool little_endian) {
    if (u8 == NULL || u32 == NULL) {
        return false;
    }

    if (little_endian == true) {
        *u32 = (uint32_t) u8[3];
        *u32 <<= 8;
        *u32 += u8[2];
        *u32 <<= 8;
        *u32 += u8[1];
        *u32 <<= 8;
        *u32 += u8[0];
    } else {
        *u32 = (uint32_t) u8[0];
        *u32 <<= 8;
        *u32 += u8[1];
        *u32 <<= 8;
        *u32 += u8[2];
        *u32 <<= 8;
        *u32 += u8[3];
    }
    return true;
}

