/*_____________________________________________________________________________________________________________________________________________________________________________________________________
 * Includes */
#include "rtc_driver.h"
#include "main.h"

#include "debug_api.h"
/*_____________________________________________________________________________________________________________________________________________________________________________________________________
 * Private definitions and macros */

/*_____________________________________________________________________________________________________________________________________________________________________________________________________
 * Private typedef */
INIT_DEBUG_MODULE(RTC)
// LSI_VALUE
#define RTC_PRESCALE_ASYNC_VALUE 31U
#define RTC_PRESCALE_SYNC_VALUE 999U
/*_____________________________________________________________________________________________________________________________________________________________________________________________________
 * Private constants */

/*_____________________________________________________________________________________________________________________________________________________________________________________________________
 * Private variables */
static char time_string[30];
static char date_string[30];
static LL_RTC_TimeTypeDef time = {0};
static LL_RTC_DateTypeDef date = {0};
static uint32_t time_ms = 0;
/*_____________________________________________________________________________________________________________________________________________________________________________________________________
 * Exported variables and references */

/*_____________________________________________________________________________________________________________________________________________________________________________________________________
 * Prototypes of private functions */

/*_____________________________________________________________________________________________________________________________________________________________________________________________________
 * Definitions of private functions */

/*_____________________________________________________________________________________________________________________________________________________________________________________________________
 * Definitions of exported functions */
bool RTC_Driver_Init (void) {
    /* ENABLE REG WRITE */
    LL_RTC_InitTypeDef RTC_InitStruct = {0};

    if (LL_RCC_GetRTCClockSource() != LL_RCC_RTC_CLKSOURCE_LSI)
    {
        LL_RCC_ForceBackupDomainReset();
        LL_RCC_ReleaseBackupDomainReset();
        LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
    }

    /* Peripheral clock enable */
    LL_RCC_EnableRTC();
    /* Initialize RTC and set the Time and Date */
    RTC_InitStruct.HourFormat = LL_RTC_HOURFORMAT_24HOUR;
    RTC_InitStruct.AsynchPrescaler = RTC_PRESCALE_ASYNC_VALUE;
    RTC_InitStruct.SynchPrescaler = RTC_PRESCALE_SYNC_VALUE;
    LL_RTC_Init(RTC, &RTC_InitStruct);
    LL_RTC_SetAsynchPrescaler(RTC, RTC_PRESCALE_ASYNC_VALUE);
    LL_RTC_SetSynchPrescaler(RTC, RTC_PRESCALE_SYNC_VALUE);

    /* Initialize RTC and set the Time and Date */
    if (LL_RTC_BAK_GetRegister(RTC, LL_RTC_BKP_DR0) != 0x32F2) {
        time.Hours = 0;
        time.Minutes = 0;
        time.Seconds = 0;
        LL_RTC_TIME_Init(RTC, LL_RTC_FORMAT_BIN, &time);
        date.WeekDay = LL_RTC_WEEKDAY_MONDAY;
        date.Month = LL_RTC_MONTH_JANUARY;
        date.Day = 0x1;
        date.Year = 0;
        LL_RTC_DATE_Init(RTC, LL_RTC_FORMAT_BIN, &date);
//        LL_RTC_BAK_SetRegister(RTC, LL_RTC_BKP_DR0, 0x32F2); // Locks register
    }
    /* DISABLE REG WRITE */
//    LL_RTC_BAK_SetRegister(RTC, LL_RTC_BKP_DR0, 0xFF);
    LL_RTC_EnableWriteProtection(RTC);
    return true;
}

bool RTC_Driver_GetTime (void) {
    uint32_t temp1 = LL_RTC_TIME_GetHour(RTC);
    uint32_t temp2 = LL_RTC_TIME_GetMinute(RTC);
    uint32_t temp3 = LL_RTC_TIME_GetSecond(RTC);

    time.Hours = (uint8_t) LL_RTC_TIME_GetHour(RTC);
    time.Minutes = (uint8_t) LL_RTC_TIME_GetMinute(RTC);
    time.Seconds = (uint8_t) LL_RTC_TIME_GetSecond(RTC);
    time_ms = LL_RTC_TIME_GetSubSecond(RTC);
    debug("RTC time: %02d:%02d  %02d.%03ds\n", time.Hours, time.Minutes, time.Seconds, time_ms);
    return true;
}

// Disable -> Write, update -> Enable
// 3UART - GPS
