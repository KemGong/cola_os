#include "config.h"
#include "gd32e23x.h"
#include "gd32_usart.h"
#include "cola_os.h"
#include "cola_device.h"
#include "cola_fifo.h"

#ifdef USING_UART1 


#ifdef USING_DEBUG
/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(COM_USART, (uint8_t) ch);
    while(RESET == usart_flag_get(COM_USART, USART_FLAG_TBE));
    return ch;
}
#endif

static cola_device_t uart0_dev;
static cola_fifo_t   uart0_fifo;
static task_t        uart0_timer;
static uint8_t       uart0_buf[1024] = {0};

static int uart0_write(cola_device_t *dev, int pos, const void *buffer, int size)
{
    com_usart_transmit_dma((uint8_t *)buffer,size);
	return size;
}
static int uart0_read(cola_device_t *dev, int pos, void *buffer, int size)
{
    return cola_fifo_read(&uart0_fifo,buffer,size);

}
static int  uart0_write_slot(uint8_t in[], uint8_t len)
{
  return  cola_fifo_write(&uart0_fifo, in, len);
}

static int uart0_config(cola_device_t *dev, void *args, void *var)
{
    struct serial_configure *cfg = (struct serial_configure *)args;
    if(cfg)
    {
        com_usart_init(cfg->baud_rate);
    }
    return 0;
}


void USART0_IRQHandler(void)
{
    /*uint8_t c;
    if (USART_GetFlagStatus(USART1, USART_FLAG_ORE))
    {
        USART_ClearFlag(USART1, USART_FLAG_ORE);
        USART_ReceiveData(USART1);
    }
    if (USART_GetITStatus(USART1, USART_IT_RXNE))
    {
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
        c = USART_ReceiveData(USART1);

        if(uart0_dev.owner)
        {
            if(uart0_write_slot( &c, 1))
            {
                cola_timer_start(&uart0_timer,TIMER_ONE_SHOT,pdMS_TO_TICKS(20));
            }
        }   	

    }*/
}


//add
void com_usart_transmit_dma(uint8_t *pTxdata, uint32_t size);

/*!
    \brief      initialize com usart
    \param[in]  none
    \param[out] none
    \retval     none
*/
void com_usart_init(uint32_t bund)
{
//  configure the GPIO peripheral

    //------------------------------------------------------------------------//
    // enable the gpio clock                                                  //
    //------------------------------------------------------------------------//
    rcu_periph_clock_enable(COM_USART_TX_GPIO_CLK);
    rcu_periph_clock_enable(COM_USART_RX_GPIO_CLK);
    //------------------------------------------------------------------------//
    // enable the gpio mode                                                   //
    //------------------------------------------------------------------------//
    gpio_mode_set(COM_USART_TX_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, COM_USART_TX_PIN);
    gpio_mode_set(COM_USART_RX_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, COM_USART_RX_PIN);

    gpio_output_options_set(COM_USART_TX_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_USART_TX_PIN);
    //gpio_output_options_set(COM_USART_RX_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_USART_RX_PIN);

    gpio_af_set(COM_USART_TX_GPIO_PORT, COM_USART_TX_GPIO_AF, COM_USART_TX_PIN);
    gpio_af_set(COM_USART_RX_GPIO_PORT, COM_USART_RX_GPIO_AF, COM_USART_RX_PIN);

// configure the DMA peripheral

    //------------------------------------------------------------------------//
    // enable DMA clock                                                       //
    //------------------------------------------------------------------------//
    rcu_periph_clock_enable(COM_USART_DMA_CLK);
    //------------------------------------------------------------------------//
    // configure USART RX DMA channel                                         //
    //------------------------------------------------------------------------//
    com_usart_receive_dma();
    
// configure the USART peripheral

    //------------------------------------------------------------------------//
    // enable USART clock                                                     //
    //------------------------------------------------------------------------//
    rcu_periph_clock_enable(COM_USART_CLK);
    //------------------------------------------------------------------------//
    // deinit the USART                                                       //
    //------------------------------------------------------------------------//
    usart_deinit(COM_USART);
    //------------------------------------------------------------------------//
    // USART configuration                                                    //
    //------------------------------------------------------------------------//
    usart_word_length_set(COM_USART, USART_WL_8BIT);
    usart_stop_bit_set(COM_USART, USART_STB_1BIT);
    usart_parity_config(COM_USART, USART_PM_NONE);
    usart_baudrate_set(COM_USART, bund);
    usart_receive_config(COM_USART, USART_RECEIVE_ENABLE);
    usart_transmit_config(COM_USART, USART_TRANSMIT_ENABLE);
    //------------------------------------------------------------------------//
    // USART DMA enable for reception                                         //
    //------------------------------------------------------------------------//
    usart_dma_receive_config(COM_USART, USART_DENR_ENABLE);
    //------------------------------------------------------------------------//
    // enable USART                                                           //
    //------------------------------------------------------------------------//
    usart_enable(COM_USART);
    //------------------------------------------------------------------------//
    // enable USART receiver timeout interrupt                                //
    //------------------------------------------------------------------------//
    usart_interrupt_enable(COM_USART, USART_INT_RT);
    //------------------------------------------------------------------------//
    // enable receiver timeout
    //------------------------------------------------------------------------//
    usart_receiver_timeout_enable(COM_USART);
    //------------------------------------------------------------------------//
    // configure receiver timeout threshold
    //------------------------------------------------------------------------//
    usart_receiver_timeout_threshold_config(COM_USART, 20);
    //------------------------------------------------------------------------//
    // configure interrupt priority                                           //
    //------------------------------------------------------------------------//
    nvic_irq_enable(COM_USART_IRQn,0);
}

void com_usart_receive_dma(void)
{
    //------------------------------------------------------------------------//
    // deinit the DMA channel                                                 //
    //------------------------------------------------------------------------//
    dma_deinit(COM_USART_RX_DMA_CHANNEL);
    //------------------------------------------------------------------------//
    // DMA Channel configuration                                              //
    //------------------------------------------------------------------------//
    dma_parameter_struct dma_init_struct;
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    //dma_init_struct.memory_addr = (uint32_t)com_usart_rx_raw_data;
    //dma_init_struct.memory_addr = (uint32_t)&uart_receive_buffer.recv_buffer[0];
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    //dma_init_struct.number = sizeof(com_usart_rx_raw_data);
    //dma_init_struct.number = sizeof(uart_receive_buffer.recv_buffer)/2;
    dma_init_struct.periph_addr = (uint32_t)(&USART_RDATA(COM_USART));
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(COM_USART_RX_DMA_CHANNEL, &dma_init_struct);
    //------------------------------------------------------------------------//
    // disable DMA circulation mode                                           //
    //------------------------------------------------------------------------//
    dma_circulation_disable(COM_USART_RX_DMA_CHANNEL);
    dma_memory_to_memory_disable(COM_USART_RX_DMA_CHANNEL);
    //------------------------------------------------------------------------//
    // enable ADC_DMA_channel                                                 //
    //------------------------------------------------------------------------//
    dma_channel_enable(COM_USART_RX_DMA_CHANNEL);
}

void com_usart_transmit_dma(uint8_t *pTxdata, uint32_t size)
{
    //------------------------------------------------------------------------//
    // deinit the DMA channel                                                 //
    //------------------------------------------------------------------------//
    dma_deinit(COM_USART_TX_DMA_CHANNEL);
    //------------------------------------------------------------------------//
    // DMA Channel configuration                                              //
    //------------------------------------------------------------------------//
    dma_parameter_struct dma_init_struct;
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr = (uint32_t)pTxdata;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = size;
    dma_init_struct.periph_addr = (uint32_t)(&USART_TDATA(COM_USART));
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_LOW;
    dma_init(COM_USART_TX_DMA_CHANNEL,&dma_init_struct);
    //------------------------------------------------------------------------//
    // disable DMA circulation mode                                           //
    //------------------------------------------------------------------------//
    dma_circulation_disable(COM_USART_TX_DMA_CHANNEL);
    dma_memory_to_memory_disable(COM_USART_TX_DMA_CHANNEL);
    //------------------------------------------------------------------------//
    // USART DMA enable for transmission                                      //
    //------------------------------------------------------------------------//
    usart_dma_transmit_config(COM_USART, USART_DENT_ENABLE);
    //------------------------------------------------------------------------//
    // enable USART_TX_DMA_channel                                            //
    //------------------------------------------------------------------------//
    dma_channel_enable(COM_USART_TX_DMA_CHANNEL);
    //while(RESET == dma_flag_get(COM_USART_TX_DMA_CHANNEL, DMA_FLAG_FTF));
}

//add_end

static struct cola_device_ops uart0_ops =
{
	.write  = uart0_write,
    .read   = uart0_read,
    .config = uart0_config,
};

static void uart_timer_cb(void *arg,uint32_t event)
{
    if(uart0_dev.owner)
    {
        cola_set_event(uart0_dev.owner,SIG_DATA);
    }
}
#endif

void board_setup_usart(void)
{
#ifdef USING_UART1
    com_usart_init(115200);
    cola_fifo_init(&uart0_fifo,uart0_buf,sizeof(uart0_buf));
    cola_timer_create(&uart0_timer,uart_timer_cb,NULL);
    uart0_dev.name = "uart1";
    uart0_dev.dops = &uart0_ops;
    cola_device_register(&uart0_dev);
#endif

}
fs_initcall(board_setup_usart);

