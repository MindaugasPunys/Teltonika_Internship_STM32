/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "led_app.h"

#include "debug_api.h"
#include "modem_api.h"
#include "heap_api.h"

#include "string.h"
#include "cmsis_os2.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
//#define SECOND_IN_MS 1000
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
typedef bool (*led_func_t) (void*);

typedef struct sLedAppStatic_t {
    led_func_t function_pointer;
} sLedAppStatic_t;

typedef struct sLedAppDynamic_t {
    eLedApiPinEnum_t led;
    osTimerId_t timer_id;
    uint16_t blink_num;
} sLedAppDynamic_t;

/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
static osThreadId_t led_app_task_handle = NULL;
static osMessageQueueId_t led_app_message_queue = NULL;
static sLedAppTaskArgs_t task_args_message = {0};

static sLedAppDynamic_t led_app_dynamic_lut[eLedApiPin_Last] = {
    [eLedApiPin_StatLed] = {.led = eLedApiPin_StatLed, .timer_id = NULL, .blink_num = 0},
    [eLedApiPin_GpsLed] = {.led = eLedApiPin_GpsLed, .timer_id = NULL, .blink_num = 0},
};

/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/
static void LED_APP_Thread (void *argument);
static bool LED_APP_TaskSet (void *task_args_struct);
static bool LED_APP_TaskReset (void *task_args_struct);
static bool LED_APP_TaskToggle (void *task_args_struct);
static bool LED_APP_TaskBlink (void *task_args_struct);
/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
INIT_DEBUG_MODULE(LED_APP)
#define MESSAGE_QUEUE_NUM 4
static const osThreadAttr_t led_app_task_attributes = {.name = "LED APP task", .priority = (osPriority_t) osPriorityNormal, .stack_size = 1024};

// @formatter:off
static const sLedAppStatic_t led_app_static_lut[eCliAppCmd_Last] = {
    [eCliAppCmd_SetLed]    = {.function_pointer = LED_APP_TaskSet},
    [eCliAppCmd_ResetLed]  = {.function_pointer = LED_APP_TaskReset},
    [eCliAppCmd_ToggleLed] = {.function_pointer = LED_APP_TaskToggle},
    [eCliAppCmd_BlinkLed]  = {.function_pointer = LED_APP_TaskBlink}
};
// @formatter:on0

/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
static void LED_APP_Thread (void *argument) {
    while (true) {
        if (osMessageQueueGet(led_app_message_queue, &task_args_message, 0U, osWaitForever) == osOK) {
            led_app_static_lut[task_args_message.task].function_pointer(task_args_message.task_args_struct);
            Heap_API_Free(task_args_message.task_args_struct);
        } else {
            error("%s\n", static_debug_message_lut[eDebugMsg_MessageQueueGetFailed]);
        }
    }
    osThreadTerminate(NULL);
}

static bool LED_APP_TaskSet (void *task_args_struct) {
    sLedAppTaskSet_t *set_args = (sLedAppTaskSet_t*) task_args_struct;
    osTimerStop(led_app_dynamic_lut[set_args->pin].timer_id);
    LED_API_SetPin(set_args->pin);
    return false;
}
static bool LED_APP_TaskReset (void *task_args_struct) {
    sLedAppTaskReset_t *reset_args = (sLedAppTaskReset_t*) task_args_struct;
    osTimerStop(led_app_dynamic_lut[reset_args->pin].timer_id);
    LED_API_ResetPin(reset_args->pin);
    return false;
}
static bool LED_APP_TaskToggle (void *task_args_struct) {
    sLedAppTaskToggle_t *toggle_args = (sLedAppTaskToggle_t*) task_args_struct;
    osTimerStop(led_app_dynamic_lut[toggle_args->pin].timer_id);
    LED_API_TogglePin(toggle_args->pin);
    return false;
}

static bool LED_APP_TaskBlink (void *task_args_struct) {
    sLedAppTaskBlink_t *blink_args = (sLedAppTaskBlink_t*) task_args_struct;
    if (blink_args->time == 0) {
        return false;
    }
    if (blink_args->frequency == 0) {
        return false;
    }

    led_app_dynamic_lut[blink_args->pin].blink_num = blink_args->time * blink_args->frequency;
    uint32_t timer_period = (uint32_t) (SECOND_IN_MS / blink_args->frequency);

    if (osTimerStart(led_app_dynamic_lut[blink_args->pin].timer_id, timer_period) != osOK) {
        error("Failed to start timer!\n");
    }
    return true;
}
static void LED_APP_BlinkPeriodic_Callback (void *argument) {
    sLedAppDynamic_t *blink_dynamic_lut = (sLedAppDynamic_t*) argument;
    LED_API_TogglePin(blink_dynamic_lut->led);
    led_app_dynamic_lut[blink_dynamic_lut->led].blink_num--;
    if (led_app_dynamic_lut[blink_dynamic_lut->led].blink_num == 0) {
        LED_API_ResetPin(blink_dynamic_lut->led);
        osTimerStop(led_app_dynamic_lut[blink_dynamic_lut->led].timer_id);
    }
}

/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool LED_APP_Init (void) {
    LED_API_Init();
    /* Create timers for each LED */
    for (eLedApiPinEnum_t led = eLedApiPin_First; led < eLedApiPin_Last; led++) {
        led_app_dynamic_lut[led].timer_id = osTimerNew(LED_APP_BlinkPeriodic_Callback,
        osTimerPeriodic, &led_app_dynamic_lut[led], NULL);

        if (led_app_dynamic_lut[eLedApiPin_Last].timer_id == NULL) {
            return false;
        }
    }

    /* Create a message queue */
    led_app_message_queue = osMessageQueueNew(10, sizeof(sLedAppTaskArgs_t), NULL);
    if (led_app_message_queue == NULL) {
        error("Failed to create message queue");
        return false;
    }

    /* Create a Thread */
    led_app_task_handle = osThreadNew(LED_APP_Thread, NULL, &led_app_task_attributes);
    if (led_app_task_handle == NULL) {
        error("Failed to initialize task\n");
        return false;
    }
    return true;
}

bool LED_APP_AddTask (sLedAppTaskArgs_t *task_struct) {
    if (task_struct == NULL) {
        return false;
    }
    if (osMessageQueuePut(led_app_message_queue, task_struct, 0U, 0U) != osOK) {
        Heap_API_Free(task_struct->task_args_struct);
        return false;
    }
    return true;
}

