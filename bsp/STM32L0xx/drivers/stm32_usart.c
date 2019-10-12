/**
  ********************************  STM32F10x  *********************************
  * @文件名     ： usart.c
  * @作者       ： sun
  * @库版本     ： V3.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2016年05月03日
  * @摘要       ： USART源文件
  ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
  2016-05-03 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
/* 包含的头文件 --------------------------------------------------------------*/
#include "config.h"
#include "stm32l0xx_hal.h"
#include "osal_device.h"
#include "osal_app.h"
#include  "config.h"
#include "alloter.h"
#include "osal.h"
#include "osal_time.h"
#include "usart.h"
struct USER_UART
{
    struct _CHN_SLOT rx_slot;
};



#ifdef USING_UART1 
static struct device uart1_dev;
char *uart1_name = "uart1";
uint8_t uart1_task = TASK_NO_TASK;
static struct USER_UART uart1_infor;


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

///************************************************
//函数名称 ： USART1_Printf
//功    能 ： 串口1打印输出
//参    数 ： string --- 字符串
//返 回 值 ： 无
//作    者 ： sun
//*************************************************/
//static void uart1_printf(uint8_t *String)
//{
//  while((*String) != '\0')
//  {
//    uart1_sendbyte(*String);
//    String++;
//  }
//}


static int uart1_write(device_t *dev,off_t pos, const void *buffer, int size)
{
	uart1_sendnbyte((uint8_t *)buffer,size);
	return size;
}

static void  uart1_slot_init(void)
{
    struct USER_UART *pchn;
    pchn                   = &uart1_infor;
    pchn->rx_slot.data_cnt = 0;
    pchn->rx_slot.tx       = INVALID_PTR;
    pchn->rx_slot.rx       = INVALID_PTR;
    pchn->rx_slot.data_max = 0xFF;
}

static int uart1_read(device_t *dev,off_t pos, void *buffer, int size)
{
    return get_chn_bytes(&uart1_infor.rx_slot, (uint8_t *)buffer, size);

}
static int uart1_peek(device_t *dev,off_t pos, void *buffer, int size)
{
    return peek_chn_bytes(&uart1_infor.rx_slot, (uint8_t *)buffer, size);
}
static int  uart1_write_slot(uint8_t in[], uint8_t len)
{
  return  put_chn_bytes(&uart1_infor.rx_slot, in, len);
}
//static int  uart1_peek_slot(uint8_t in[], uint8_t len)
//{
//  return  peek_chn_bytes(&uart1_infor.rx_slot, in, len);
//}

static void USART1_RECV_IRQHandler(UART_HandleTypeDef *huart)
{
    uint8_t c;
    c = (uint8_t)(huart->Instance->RDR);
    if(uart1_write_slot( &c, 1))
    {
        if(TASK_NO_TASK != uart1_task)
        {
            osal_start_timerEx(uart1_task, SIG_UART, 100);
        }
    }
    else
    {
        if(TASK_NO_TASK != uart1_task)
        {
            osal_stop_timerEx(uart1_task, SIG_UART);
            osal_set_event(uart1_task, SIG_UART);
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
static int uart1_config(device_t *dev,void *args,int pos)
{
    struct serial_configure *cfg = (struct serial_configure *)args;
    uart1_configuration(cfg->baud_rate);
    uart1_task = (uint8_t)pos;
    return 0;
}
static struct device_ops uart1_ops =
{
	.write  = uart1_write,
    .read   = uart1_read,
    .peek   = uart1_peek,
    .config = uart1_config,
};





#endif
int board_setup_usart(void)
{
#ifdef USING_UART1
    uart1_configuration(115200);
    uart1_slot_init();
    uart1_dev.name = uart1_name;
    uart1_dev.ops = &uart1_ops;
    device_register(&uart1_dev);
#endif
    return 0;
}
device_initcall(board_setup_usart);

