#include "config.h"
#include "stm32l0xx_hal.h"
#include "cola_os.h"
#include "cola_device.h"
#include "usart.h"
#include "cola_fifo.h"


#ifdef USING_UART1 


#ifdef USING_DEBUG
int fputc(int ch, FILE *f)
{      
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFF);
    return ch;
}
#endif


static cola_device_t uart1_dev;
static cola_fifo_t   uart1_fifo;
static task_t        uart1_timer;
static uint8_t       uart1_buf[1024] = {0};

static void uart1_configuration(uint32_t bund)
{
    MX_USART1_UART_Init();
    huart1.Init.BaudRate = bund;
    HAL_UART_Init(&huart1);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_ERR);
}

/************************************************
函数名称 ： USART1_SendNByte
功    能 ： 串口1发送N个字符
参    数 ： pData ----- 字符串
            Length --- 长度
返 回 值 ： 无
作    者 ： sun
*************************************************/
static void uart1_sendnbyte(uint8_t *pData, uint16_t Length)
{
    HAL_UART_Transmit(&huart1,pData,Length,0xFF);
}

static int uart1_write(cola_device_t *dev, int pos, const void *buffer, int size)
{
	uart1_sendnbyte((uint8_t *)buffer,size);
	return size;
}
static int uart1_read(cola_device_t *dev, int pos, void *buffer, int size)
{
    return cola_fifo_read(&uart1_fifo,buffer,size);
}
static int  uart1_write_slot(uint8_t in[], uint8_t len)
{
  return  cola_fifo_write(&uart1_fifo, in, len);
}

static int uart1_config(cola_device_t *dev, void *args, void *var)
{
    struct serial_configure *cfg = (struct serial_configure *)args;
    if(cfg)
    {
        uart1_configuration(cfg->baud_rate);
    }
    return 0;
}

static void USART1_RECV_IRQHandler(UART_HandleTypeDef *huart)
{
    uint8_t c;
    c = (uint8_t)(huart->Instance->RDR);
    if(uart1_dev.owner)
    {
        if(uart1_write_slot( &c, 1))
        {
            cola_timer_start(&uart1_timer,TIMER_ONE_SHOT,pdMS_TO_TICKS(20));
        }
    }	
}

static void USER_UART1_IRQHandler(UART_HandleTypeDef *huart)
{
  uint32_t isrflags   = READ_REG(huart->Instance->ISR);
  uint32_t cr1its     = READ_REG(huart->Instance->CR1);
  uint32_t cr3its     = READ_REG(huart->Instance->CR3);

  uint32_t errorflags;
  uint32_t errorcode;

  /* If no error occurs */
  errorflags = (isrflags & (uint32_t)(USART_ISR_PE | USART_ISR_FE | USART_ISR_ORE | USART_ISR_NE));
  if (errorflags == 0U)
  {
    /* UART in mode Receiver ---------------------------------------------------*/
    if (((isrflags & USART_ISR_RXNE) != 0U)
        && ((cr1its & USART_CR1_RXNEIE) != 0U))
    {
      if (huart->RxISR != NULL)
      {
        huart->RxISR(huart);
        
      }
      USART1_RECV_IRQHandler(huart);
      return;
    }
  }

  /* If some errors occur */
  if ((errorflags != 0U)
      && (((cr3its & USART_CR3_EIE) != 0U)
          || ((cr1its & (USART_CR1_RXNEIE | USART_CR1_PEIE)) != 0U)))
  {
    /* UART parity error interrupt occurred -------------------------------------*/
    if (((isrflags & USART_ISR_PE) != 0U) && ((cr1its & USART_CR1_PEIE) != 0U))
    {
      __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_PEF);

      huart->ErrorCode |= HAL_UART_ERROR_PE;
    }

    /* UART frame error interrupt occurred --------------------------------------*/
    if (((isrflags & USART_ISR_FE) != 0U) && ((cr3its & USART_CR3_EIE) != 0U))
    {
      __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_FEF);

      huart->ErrorCode |= HAL_UART_ERROR_FE;
    }

    /* UART noise error interrupt occurred --------------------------------------*/
    if (((isrflags & USART_ISR_NE) != 0U) && ((cr3its & USART_CR3_EIE) != 0U))
    {
      __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_NEF);

      huart->ErrorCode |= HAL_UART_ERROR_NE;
    }

    /* UART Over-Run interrupt occurred -----------------------------------------*/
    if (((isrflags & USART_ISR_ORE) != 0U)
        && (((cr1its & USART_CR1_RXNEIE) != 0U) ||
            ((cr3its & USART_CR3_EIE) != 0U)))
    {
      __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF);

      huart->ErrorCode |= HAL_UART_ERROR_ORE;
    }

    /* Call UART Error Call back function if need be --------------------------*/
    if (huart->ErrorCode != HAL_UART_ERROR_NONE)
    {
      /* UART in mode Receiver ---------------------------------------------------*/
      if (((isrflags & USART_ISR_RXNE) != 0U)
          && ((cr1its & USART_CR1_RXNEIE) != 0U))
      {
        if (huart->RxISR != NULL)
        {
          huart->RxISR(huart);
        }
      }

      /* If Overrun error occurs, or if any error occurs in DMA mode reception,
         consider error as blocking */
      errorcode = huart->ErrorCode;
      if ((HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR)) ||
          ((errorcode & HAL_UART_ERROR_ORE) != 0U))
      {
        /* Blocking error : transfer is aborted
           Set the UART state ready to be able to start again the process,
           Disable Rx Interrupts, and disable Rx DMA request, if ongoing */
        //UART_EndRxTransfer(huart);

        /* Disable the UART DMA Rx request if enabled */
        if (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR))
        {
          CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);

          /* Abort the UART DMA Rx channel */
          if (huart->hdmarx != NULL)
          {
            /* Set the UART DMA Abort callback :
               will lead to call HAL_UART_ErrorCallback() at end of DMA abort procedure */
           // huart->hdmarx->XferAbortCallback = UART_DMAAbortOnError;

            /* Abort DMA RX */
            if (HAL_DMA_Abort_IT(huart->hdmarx) != HAL_OK)
            {
              /* Call Directly huart->hdmarx->XferAbortCallback function in case of error */
              huart->hdmarx->XferAbortCallback(huart->hdmarx);
            }
          }
          else
          {
            /* Call user error callback */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
            /*Call registered error callback*/
            huart->ErrorCallback(huart);
#else
            /*Call legacy weak error callback*/
            HAL_UART_ErrorCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

          }
        }
        else
        {
          /* Call user error callback */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
          /*Call registered error callback*/
          huart->ErrorCallback(huart);
#else
          /*Call legacy weak error callback*/
          HAL_UART_ErrorCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
        }
      }
      else
      {
        /* Non Blocking error : transfer could go on.
           Error is notified to user through user error callback */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
        /*Call registered error callback*/
        huart->ErrorCallback(huart);
#else
        /*Call legacy weak error callback*/
        HAL_UART_ErrorCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
        huart->ErrorCode = HAL_UART_ERROR_NONE;
      }
    }
    return;

  } /* End if some error occurs */

  /* UART wakeup from Stop mode interrupt occurred ---------------------------*/
  if (((isrflags & USART_ISR_WUF) != 0U) && ((cr3its & USART_CR3_WUFIE) != 0U))
  {
    __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_WUF);

    /* UART Rx state is not reset as a reception process might be ongoing.
       If UART handle state fields need to be reset to READY, this could be done in Wakeup callback */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
    /* Call registered Wakeup Callback */
    huart->WakeupCallback(huart);
#else
    /* Call legacy weak Wakeup Callback */
    HAL_UARTEx_WakeupCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
    return;
  }

  /* UART in mode Transmitter ------------------------------------------------*/
  if (((isrflags & USART_ISR_TXE) != 0U)
      && ((cr1its & USART_CR1_TXEIE) != 0U))
  {
    if (huart->TxISR != NULL)
    {
      huart->TxISR(huart);
    }
    return;
  }

  /* UART in mode Transmitter (transmission end) -----------------------------*/
  if (((isrflags & USART_ISR_TC) != 0U) && ((cr1its & USART_CR1_TCIE) != 0U))
  {
   // UART_EndTransmit_IT(huart);
    return;
  }

}

void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  USER_UART1_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}
static struct cola_device_ops uart1_ops =
{
	.write  = uart1_write,
    .read   = uart1_read,
    .config = uart1_config,
};

static void uart_timer_cb(void *arg,uint32_t event)
{
    if(uart1_dev.owner)
    {
        cola_set_event(uart1_dev.owner,SIG_DATA);
    }
}
#endif


void board_setup_usart(void)
{
#ifdef USING_UART1
    uart1_configuration(115200);
    cola_fifo_init(&uart1_fifo,uart1_buf,sizeof(uart1_buf));
    cola_timer_create(&uart1_timer,uart_timer_cb,NULL);
    uart1_dev.name = "uart1";
    uart1_dev.dops = &uart1_ops;
    cola_device_register(&uart1_dev);
#endif

}
fs_initcall(board_setup_usart);
