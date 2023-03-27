#ifndef __CIRC_BUFFER__H__
#define __CIRC_BUFFER__H__

/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "main.h"
#include "stdbool.h"

/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/

// Opaque circular buffer structure
typedef struct circular_buf_t circular_buf_t;

// Handle type, the way users interact with the API
typedef circular_buf_t *cbuf_handle_t;

/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/

/// Pass in a storage buffer and size
/// Returns a circular buffer handle
cbuf_handle_t CIRC_Buffer_init (size_t size);

/// Free a circular buffer structure.
/// Does not free data buffer; owner is responsible for that
bool CIRC_Buffer_free (cbuf_handle_t me);

/// Reset the circular buffer to empty, head == tail
bool CIRC_Buffer_reset (cbuf_handle_t me);

/// Put version 1 continues to add data if the buffer is full
/// Old data is overwritten
bool CIRC_Buffer_put (cbuf_handle_t me, uint8_t data);

/// Put Version 2 rejects new data if the buffer is full
/// Returns 0 on success, -1 if buffer is full
bool CIRC_Buffer_put2 (cbuf_handle_t me, uint8_t data);

/// Retrieve a value from the buffer
/// Returns 0 on success, -1 if the buffer is empty
bool CIRC_Buffer_get (cbuf_handle_t me, uint8_t *data);

/// Returns true if the buffer is empty
bool CIRC_Buffer_empty (cbuf_handle_t me);

/// Returns true if the buffer is full
bool CIRC_Buffer_full (cbuf_handle_t me);

/// Returns the maximum capacity of the buffer
size_t CIRC_Buffer_capacity (cbuf_handle_t me);

/// Returns the current number of elements in the buffer
size_t CIRC_Buffer_size (cbuf_handle_t me);

#endif /* __CIRC_BUFFER__H__ */
