#include "config.h"
#include "stm32f0xx_conf.h"
#include "cola_os.h"
#include "cola_device.h"
#include "cola_fifo.h"

#ifdef USING_UART1 


#ifdef USING_DEBUG
int fputc(int ch, FILE *f)
{      
	while((USART1->ISR & USART_FLAG_TXE) == RESET);
    USART1->TDR = ((ch) & (uint16_t)0x01FF);
	return ch;
}
#endif

static cola_device_t uart1_dev;
static cola_fifo_t   uart1_fifo;
static task_t        uart1_timer;
static uint8_t       uart1_buf[1024] = {0};

static void uart1_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;           
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;                       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                     
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;                       
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}


static void uart1_init(uint32_t bund)
{
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    USART_InitStructure.USART_BaudRate   = bund;                       
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;        
    USART_InitStructure.USART_StopBits   = USART_StopBits_1;            
    USART_InitStructure.USART_Parity     = USART_Parity_No ;               
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;    
    USART_Init(USART1, &USART_InitStructure);                          

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                     

    USART_Cmd(USART1, ENABLE);                                         
}
static void uart1_nvic_init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;                  
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
static void uart1_configuration(uint32_t bund)
{
    uart1_gpio_init();
    uart1_init(bund);
    uart1_nvic_init();
}
void USART1_SendByte(uint8_t Data)
{
    while((USART1->ISR & USART_FLAG_TXE) == RESET);
    USART1->TDR = (Data & (uint16_t)0x01FF);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
}

static void uart1_sendnbyte(uint8_t *pData, uint16_t Length)
{
  while(Length--)
  {
    USART1_SendByte(*pData);
    pData++;
  }
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
        uart1_init(cfg->baud_rate);
    }
    return 0;
}


void USART1_IRQHandler(void)
{
    uint8_t c;
    if (USART_GetFlagStatus(USART1, USART_FLAG_ORE))
    {
        USART_ClearFlag(USART1, USART_FLAG_ORE);
        USART_ReceiveData(USART1);
    }
    if (USART_GetITStatus(USART1, USART_IT_RXNE))
    {
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
        c = USART_ReceiveData(USART1);

        if(uart1_dev.owner)
        {
            if(uart1_write_slot( &c, 1))
            {
                cola_timer_start(&uart1_timer,TIMER_ONE_SHOT,pdMS_TO_TICKS(20));
            }
        }   	

    }
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

