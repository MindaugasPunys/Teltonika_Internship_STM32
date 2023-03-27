#ifndef __STRING_UTILS__H__
#define __STRING_UTILS__H__

/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "stdbool.h"
#include "stdint.h"
#include "stddef.h"
/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/
#define getName(var)  #var
/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
bool StrContains (const char *string, size_t string_size, const char *chars);
bool CharAt (const char *string, int index);
bool StrRemChar (char *source, const char *chars);
bool StrRemSubstring (char *source, const char *substring);
bool StrCompare (const char *string1, const char *string2);
bool StrIsNumber (const char *string);
bool StrToUint (const char *string, uint16_t *value);
bool StrToHex (const char *string, uint16_t string_length, char **hex_string);
bool StructToHex (void* structure, size_t struct_size, char **hex_string);
bool StrHexToStr (const char *hex_string, uint16_t hex_string_length, char **string);

char* StrLastLine (char *string);
char* StrLastSubstring (char *string, char *substring);
int StrCountChars (const char *string, size_t string_size, const char *chars);

bool StrUint32ToUint8 (uint32_t u32, uint8_t *u8, bool little_endian);
bool StrUint8ToUint32 (uint8_t *u8, uint32_t *u32, bool little_endian);
#endif /* __STRING_UTILS__H__ */
