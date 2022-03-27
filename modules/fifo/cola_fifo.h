
#ifndef COLA_FIFO_H__
#define COLA_FIFO_H__

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**@brief   A FIFO instance structure.
 * @details Keeps track of which bytes to read and write next.
 *          Also, it keeps the information about which memory is allocated for the buffer
 *          and its size. This structure must be initialized by app_fifo_init() before use.
 */
typedef struct
{
    uint8_t *          p_buf;           /**< Pointer to FIFO buffer memory.                      */
    uint16_t           buf_size_mask;   /**< Read/write index mask. Also used for size checking. */
    volatile uint32_t  read_pos;        /**< Next read position in the FIFO buffer.              */
    volatile uint32_t  write_pos;       /**< Next write position in the FIFO buffer.             */
} cola_fifo_t;

/**@brief Function for initializing the FIFO.
 *
 * @param[out] p_fifo   FIFO object.
 * @param[in]  p_buf    FIFO buffer for storing data. The buffer size must be a power of two.
 * @param[in]  buf_size Size of the FIFO buffer provided. This size must be a power of two.
 *
 * @retval     1        If initialization was successful.
 * @retval     0        If a NULL pointer is provided as buffer.
 * @retval     0        If size of buffer provided is not a power of two.
 */
uint32_t cola_fifo_init(cola_fifo_t * p_fifo, uint8_t * p_buf, uint16_t buf_size);

/**@brief Function for adding an element to the FIFO.
 *
 * @param[in]  p_fifo   Pointer to the FIFO.
 * @param[in]  byte     Data byte to add to the FIFO.
 *
 * @retval     1        If an element has been successfully added to the FIFO.
 * @retval     0        If the FIFO is full.
 */
uint32_t cola_fifo_put(cola_fifo_t * p_fifo, uint8_t byte);

/**@brief Function for getting the next element from the FIFO.
 *
 * @param[in]  p_fifo   Pointer to the FIFO.
 * @param[out] p_byte   Byte fetched from the FIFO.
 *
 * @retval     1        If an element was returned.
 * @retval     0        If there are no more elements in the queue.
 */
uint32_t cola_fifo_get(cola_fifo_t * p_fifo, uint8_t * p_byte);

/**@brief Function for looking at an element in the FIFO, without consuming it.
 *
 * @param[in]  p_fifo   Pointer to the FIFO.
 * @param[in]  index    Which element to look at. The lower the index, the earlier it was put.
 * @param[out] p_byte   Byte fetched from the FIFO.
 *
 * @retval     1        If an element was returned.
 * @retval     0        If there are no more elements in the queue, or the index was
 *                                      too large.
 */
uint32_t cola_fifo_peek(cola_fifo_t * p_fifo, uint8_t * p_byte_array, uint32_t  p_size);

/**@brief Function for flushing the FIFO.
 *
 * @param[in]  p_fifo   Pointer to the FIFO.
 *
 * @retval     1              If the FIFO was flushed successfully.
 */
uint32_t cola_fifo_flush(cola_fifo_t * p_fifo);

/**@brief Function for reading bytes from the FIFO.
 *
 * This function can also be used to get the number of bytes in the FIFO.
 *
 * @param[in]    p_fifo        Pointer to the FIFO. Must not be NULL.
 * @param[out]   p_byte_array  Memory pointer where the read bytes are fetched from the FIFO.
 *                             Can be NULL. If NULL, the number of bytes that can be read in the FIFO
 *                             are returned in the p_size parameter.
 * @param[in] p_size        Address to memory indicating the maximum number of bytes to be read.
 *                             The provided memory is overwritten with the actual number of bytes
 *                             read if the procedure was successful. This field must not be NULL.
 *                             If p_byte_array is set to NULL by the application, this parameter
 *                             returns the number of bytes in the FIFO.
 *
 * @retval     size       If the procedure is successful. The actual number of bytes read might
 *                                  be less than the requested maximum, depending on how many elements exist
 *                                  in the FIFO. Even if less bytes are returned, the procedure is considered
 *                                  successful.
 * @retval     0          If a NULL parameter was passed for a parameter that must not
 *                                  be NULL.
 * @retval     0          If the FIFO is empty.
 */
uint32_t cola_fifo_read(cola_fifo_t * p_fifo, uint8_t * p_byte_array, uint32_t  p_size);

/**@brief Function for writing bytes to the FIFO.
 *
 * This function can also be used to get the available size on the FIFO.
 *
 * @param[in]  p_fifo       Pointer to the FIFO. Must not be NULL.
 * @param[in]  p_byte_array Memory pointer containing the bytes to be written to the FIFO.
 *                          Can be NULL. If NULL, this function returns the number of bytes
 *                          that can be written to the FIFO.
 * @param[in]  p_size     Address to memory indicating the maximum number of bytes to be written.
 *                          The provided memory is overwritten with the number of bytes that were actually
 *                          written if the procedure is successful. This field must not be NULL.
 *                          If p_byte_array is set to NULL by the application, this parameter
 *                          returns the number of bytes available in the FIFO.
 *
 * @retval     size       If the procedure is successful. The actual number of bytes written might
 *                               be less than the requested maximum, depending on how much room there is in
 *                               the FIFO. Even if less bytes are written, the procedure is considered
 *                               successful. If the write was partial, the application should use
 *                               subsequent calls to attempt writing the data again.
 * @retval     0         If a NULL parameter was passed for a parameter that must not
 *                               be NULL.
 * @retval     0         If the FIFO is full.
 *
 */
uint32_t cola_fifo_write(cola_fifo_t * p_fifo, const uint8_t  * p_byte_array, uint32_t  p_size);


uint32_t fifo_avail(cola_fifo_t * p_fifo);

#ifdef __cplusplus
}
#endif

#endif // APP_FIFO_H__

/** @} */
