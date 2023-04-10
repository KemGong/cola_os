#ifndef _GD32_USART_H_
#define _GD32_USART_H_
#include "gd32e23x.h"

#define USE_COM_USART
#ifdef USE_COM_USART
    //------------------------------------------------------------------------//
    // com usart                                                              //
    //------------------------------------------------------------------------//
    #define COM_USART                                   USART0
    #define COM_USART_BAUD                              (115200)
    #define COM_USART_CLK                               RCU_USART0
    #define COM_USART_IRQn                              USART0_IRQn
    
    #define COM_USART_TX_PIN                            GPIO_PIN_9
    #define COM_USART_TX_GPIO_PORT                      GPIOA
    #define COM_USART_TX_GPIO_CLK                       RCU_GPIOA
    #define COM_USART_TX_GPIO_AF                        GPIO_AF_1

    #define COM_USART_RX_PIN                            GPIO_PIN_10
    #define COM_USART_RX_GPIO_PORT                      GPIOA
    #define COM_USART_RX_GPIO_CLK                       RCU_GPIOA
    #define COM_USART_RX_GPIO_AF                        GPIO_AF_1

    //------------------------------------------------------------------------//
    // com usart dma                                                          //
    //------------------------------------------------------------------------//
    #define COM_USART_DMA_CLK                           RCU_DMA
    #define COM_USART_TX_DMA_CHANNEL                    DMA_CH1
    #define COM_USART_RX_DMA_CHANNEL                    DMA_CH2
    #define COM_USART_TX_DMA_CHANNEL_IRQn               DMA_Channel1_2_IRQn
    #define COM_USART_RX_DMA_CHANNEL_IRQn               DMA_Channel1_2_IRQn

    void com_usart_init(uint32_t bund);
    void com_usart_transmit_dma(uint8_t *pTxdata, uint32_t size);
    void com_usart_receive_dma(void);
    //extern uint8_t  com_usart_rx_raw_data[256];
    //extern uint8_t  com_usart_tx_raw_data[40];

#endif

#endif
