/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "heap_api.h"
#include "stdlib.h"
#include "debug_api.h"
#include "cmsis_os2.h"

/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
//#define ENABLE_MEM_BLOCK_PRINTS
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
static const osMutexAttr_t mutex_attributes = {"heap_api_mutex", 0, NULL, 0U};
/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
static uint16_t used_memory_blocks = 0;
INIT_DEBUG_MODULE(HEAP_API)
static osMutexId_t heap_api_mutex_id;
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

/* Init Heap_API MUTEX */
bool Heap_API_Init () {
    heap_api_mutex_id = osMutexNew(&mutex_attributes);
    if (heap_api_mutex_id == NULL) {
        error("Failed to create MUTEX\n");
        return false;
    }
    return true;
}

// Interface for:
// void* calloc (size_t num, size_t size);
void* Heap_API_Allocate (size_t num, size_t size) {
    /* ASSERT */
    if (num == 0) {
        error("Memory allocate argument num == 0\n");
        return NULL;
    }
    if (size == 0) {
        error("Memory allocate argument size == 0\n");
        return NULL;
    }
    /* TAKE MUTEX */
    if (osMutexAcquire(heap_api_mutex_id, osWaitForever) != osOK) {
        error("Failed to take MUTEX\n");
        return NULL;
    }

    /* calloc */
    void *pointer = NULL;
    pointer = calloc(num, size);
    if (pointer != NULL) {
        used_memory_blocks++;
    }
#ifdef ENABLE_MEM_BLOCK_PRINTS
    debug("Allocated memory blocks: %u\n", used_memory_blocks);
#endif
    /* GIVE MUTEX */
    if (osMutexRelease(heap_api_mutex_id) != osOK) {
        error("Failed to give MUTEX\n");
    }
    return pointer;
}

// Interface for:
// void free (void* ptr);
bool Heap_API_Free (void *pointer) {
    /* ASSERT */
    if (pointer == NULL) {
        error("Argument == NULL\n");
        return false;
    }

    /* TAKE MUTEX */
    if (osMutexAcquire(heap_api_mutex_id, osWaitForever) != osOK) {
        error("Failed to take MUTEX\n");
        return NULL;
    }
    free(pointer);
    used_memory_blocks--;

    /* GIVE MUTEX */
    if (osMutexRelease(heap_api_mutex_id) != osOK) {
        error("Failed to give MUTEX\n");
        return false;
    }

#ifdef ENABLE_MEM_BLOCK_PRINTS
    debug("Allocated memory blocks: %u\n", used_memory_blocks);
#endif

    return true;
}

// Return Used memory blocks count
uint16_t Heap_API_BlockCount () {
    return used_memory_blocks;
}
