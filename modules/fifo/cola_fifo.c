#include "cola_fifo.h"
#include "utils.h"
static __inline uint32_t fifo_length(cola_fifo_t * p_fifo)
{
    uint32_t tmp = p_fifo->read_pos;
    return p_fifo->write_pos - tmp;
}


#define FIFO_LENGTH() fifo_length(p_fifo)  /**< Macro for calculating the FIFO length. */


/**@brief Put one byte to the FIFO. */
static __inline void fifo_put(cola_fifo_t * p_fifo, uint8_t byte)
{
    p_fifo->p_buf[p_fifo->write_pos & p_fifo->buf_size_mask] = byte;
    p_fifo->write_pos++;
}


/**@brief Look at one byte in the FIFO. */
static __inline void fifo_peek(cola_fifo_t * p_fifo, uint16_t index, uint8_t * p_byte)
{
    *p_byte = p_fifo->p_buf[(p_fifo->read_pos + index) & p_fifo->buf_size_mask];
}


/**@brief Get one byte from the FIFO. */
static __inline void fifo_get(cola_fifo_t * p_fifo, uint8_t * p_byte)
{
    fifo_peek(p_fifo, 0, p_byte);
    p_fifo->read_pos++;
}


uint32_t cola_fifo_init(cola_fifo_t * p_fifo, uint8_t * p_buf, uint16_t buf_size)
{
    // Check buffer for null pointer.
    if (p_buf == NULL)
    {
        return 0;
    }

    // Check that the buffer size is a power of two.
    if (!IS_POWER_OF_TWO(buf_size))
    {
        return 0;
    }

    p_fifo->p_buf         = p_buf;
    p_fifo->buf_size_mask = buf_size - 1;
    p_fifo->read_pos      = 0;
    p_fifo->write_pos     = 0;

    return 1;
}


uint32_t cola_fifo_put(cola_fifo_t * p_fifo, uint8_t byte)
{
    if (FIFO_LENGTH() <= p_fifo->buf_size_mask)
    {
        fifo_put(p_fifo, byte);
        return 1;
    }

    return 0;
}


uint32_t cola_fifo_get(cola_fifo_t * p_fifo, uint8_t * p_byte)
{
    if (FIFO_LENGTH() != 0)
    {
        fifo_get(p_fifo, p_byte);
        return 1;
    }

    return 0;

}


uint32_t cola_fifo_peek(cola_fifo_t * p_fifo, uint8_t * p_byte_array, uint32_t  p_size)
{
    if(p_fifo == NULL)
    {
        return 0;
    }
    if(p_size == 0)
    {
        return 0;
    }
    const uint32_t byte_count    = fifo_length(p_fifo);
    const uint32_t requested_len = (p_size);
    uint32_t       index         = 0;
    uint32_t       read_size     = MIN(requested_len, byte_count);

    (p_size) = byte_count;

    // Check if the FIFO is empty.
    if (byte_count == 0)
    {
        return 0;
    }

    // Check if application has requested only the size.
    if (p_byte_array == NULL)
    {
        return 0;
    }

    // Fetch bytes from the FIFO.
    while (index < read_size)
    {
        fifo_peek(p_fifo,index,&p_byte_array[index]);
        index++;
    }

    (p_size) = read_size;

    return p_size;
}



uint32_t cola_fifo_flush(cola_fifo_t * p_fifo)
{
    p_fifo->read_pos = p_fifo->write_pos;
    return 1;
}


uint32_t cola_fifo_read(cola_fifo_t * p_fifo, uint8_t * p_byte_array, uint32_t  p_size)
{
    //VERIFY_PARAM_NOT_NULL(p_fifo);
    //VERIFY_PARAM_NOT_NULL(p_size);
    if(p_fifo == NULL)
    {
        return 0;
    }
    if(p_size == 0)
    {
        return 0;
    }
    const uint32_t byte_count    = fifo_length(p_fifo);
    const uint32_t requested_len = (p_size);
    uint32_t       index         = 0;
    uint32_t       read_size     = MIN(requested_len, byte_count);

    (p_size) = byte_count;

    // Check if the FIFO is empty.
    if (byte_count == 0)
    {
        return 0;
    }

    // Check if application has requested only the size.
    if (p_byte_array == NULL)
    {
        return 0;
    }

    // Fetch bytes from the FIFO.
    while (index < read_size)
    {
        fifo_get(p_fifo, &p_byte_array[index++]);
    }

    (p_size) = read_size;

    return p_size;
}


uint32_t cola_fifo_write(cola_fifo_t * p_fifo, const uint8_t  * p_byte_array, uint32_t  p_size)
{
    //VERIFY_PARAM_NOT_NULL(p_fifo);
    //VERIFY_PARAM_NOT_NULL(p_size);
    if(p_fifo == NULL)
    {
        return 0;
    }
    if(p_size == 0)
    {
        return 0;
    }
    const uint32_t available_count = p_fifo->buf_size_mask - fifo_length(p_fifo) + 1;
    const uint32_t requested_len   = (p_size);
    uint32_t       index           = 0;
    uint32_t       write_size      = MIN(requested_len, available_count);

    (p_size) = available_count;

    // Check if the FIFO is FULL.
    if (available_count == 0)
    {
        return 0;
    }

    // Check if application has requested only the size.
    if (p_byte_array == NULL)
    {
        return 0;
    }

    //Fetch bytes from the FIFO.
    while (index < write_size)
    {
        fifo_put(p_fifo, p_byte_array[index++]);
    }

    (p_size) = write_size;

    return p_size;
}

uint32_t fifo_avail(cola_fifo_t * p_fifo)
{
    return p_fifo->buf_size_mask+1-fifo_length(p_fifo);
}
