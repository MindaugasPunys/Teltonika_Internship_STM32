/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "circ_buffer.h"
#include "stdlib.h"

/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/

// The hidden definition of our circular buffer structure
struct circular_buf_t {
    uint8_t *buffer;
    size_t head;
    size_t tail;
    size_t size;
    bool full;
};

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
static bool Circ_Buffer_advance_pointer (cbuf_handle_t cbuf_handle);
static bool CIRC_Buffer_retreat_pointer (cbuf_handle_t cbuf_handle);
/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
static bool Circ_Buffer_advance_pointer (cbuf_handle_t cbuf_handle) {
    if (cbuf_handle == NULL) {
        return false;
    }

    if (cbuf_handle->full) {
        if (++(cbuf_handle->tail) == cbuf_handle->size) {
            cbuf_handle->tail = 0;
        }
    }

    if (++(cbuf_handle->head) == cbuf_handle->size) {
        cbuf_handle->head = 0;
    }
    cbuf_handle->full = (cbuf_handle->head == cbuf_handle->tail);
    return true;
}

static bool CIRC_Buffer_retreat_pointer (cbuf_handle_t cbuf_handle) {
    if (cbuf_handle == NULL) {
        return false;
    }

    cbuf_handle->full = false;
    if (++(cbuf_handle->tail) == cbuf_handle->size)
        {
        cbuf_handle->tail = 0;
    }
    return true;
}

/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
cbuf_handle_t CIRC_Buffer_init (size_t size) {
    cbuf_handle_t cbuf_handle = calloc(1, sizeof(circular_buf_t));
    if (cbuf_handle == NULL) {
        return NULL;
    }

    cbuf_handle->buffer = calloc(size, sizeof(uint8_t));
    if (cbuf_handle->buffer == NULL) {
        free(cbuf_handle);
        return NULL;
    }
    cbuf_handle->size = size;
    CIRC_Buffer_reset(cbuf_handle);

    return cbuf_handle;
}

bool CIRC_Buffer_reset (cbuf_handle_t cbuf_handle) {
    if (cbuf_handle == NULL) {
        return false;
    }

    cbuf_handle->head = 0;
    cbuf_handle->tail = 0;
    cbuf_handle->full = false;

    return true;
}


bool CIRC_Buffer_free (cbuf_handle_t cbuf_handle) {
    if (cbuf_handle == NULL) {
        return false;
    }
    free(cbuf_handle);

    return true;
}


bool CIRC_Buffer_full (cbuf_handle_t cbuf_handle) {
    if (cbuf_handle == NULL) {
        return false;
    }
    return cbuf_handle->full;
}

bool CIRC_Buffer_empty (cbuf_handle_t cbuf_handle) {
    if (cbuf_handle == NULL) {
        return false;
    }
    return (!cbuf_handle->full && (cbuf_handle->head == cbuf_handle->tail));
}

size_t CIRC_Buffer_capacity (cbuf_handle_t cbuf_handle) {
    if (cbuf_handle == NULL) {
        return 0;
    }
    return cbuf_handle->size;
}

size_t CIRC_Buffer_size (cbuf_handle_t cbuf_handle) {
    if (cbuf_handle == NULL) {
        return 0;
    }

    size_t size = cbuf_handle->size;

    if (!cbuf_handle->full) {
        if (cbuf_handle->head >= cbuf_handle->tail) {
            size = (cbuf_handle->head - cbuf_handle->tail);
        }
        else {
            size = (cbuf_handle->size + cbuf_handle->head - cbuf_handle->tail);
        }
    }

    return size;
}

bool CIRC_Buffer_put (cbuf_handle_t cbuf_handle, uint8_t data) {
    if (cbuf_handle == NULL) {
        return false;
    }
    cbuf_handle->buffer[cbuf_handle->head] = data;
    return Circ_Buffer_advance_pointer(cbuf_handle);
}

bool CIRC_Buffer_put2 (cbuf_handle_t cbuf_handle, uint8_t data) {
    bool r = false;

    if (cbuf_handle == NULL) {
        return r;
    }

    if (!CIRC_Buffer_full(cbuf_handle)) {
        cbuf_handle->buffer[cbuf_handle->head] = data;
        r = Circ_Buffer_advance_pointer(cbuf_handle);
    }

    return r;
}

bool CIRC_Buffer_get (cbuf_handle_t cbuf_handle, uint8_t *data) {
    bool ret = false;
    if (cbuf_handle == NULL || data == NULL) {
        return ret;
    }

    if (!CIRC_Buffer_empty(cbuf_handle)) {
        *data = cbuf_handle->buffer[cbuf_handle->tail];
        ret = CIRC_Buffer_retreat_pointer(cbuf_handle);
    }

    return ret;
}
