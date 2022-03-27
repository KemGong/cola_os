#include "config.h"
#include "SWM181.h"
#include "cola_device.h"
#include "cola_init.h"
#include "utils.h"
#include "cola_fifo.h"
#include "cola_os.h"

#ifdef USING_UART0 

static cola_device_t uart0_dev;
static cola_fifo_t   uart0_fifo;
static task_t        uart0_timer;
static uint8_t       uart0_buf[1024] = {0};


int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}


static void uart0_init(uint32_t bund)
{
    UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTA, PIN0, FUNMUX_UART0_RXD, 1);	//GPIOA.0配置为UART0输入引脚
	PORT_Init(PORTA, PIN1, FUNMUX_UART0_TXD, 0);	//GPIOA.1配置为UART0输出引脚
 	
 	UART_initStruct.Baudrate = bund;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity   = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThreshold = 3;
	UART_initStruct.RXThresholdIEn = 1;
	UART_initStruct.TXThreshold = 3;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutTime = 10;		//10个字符时间内未接收到新的数据则触发超时中断
	UART_initStruct.TimeoutIEn = 1;
 	UART_Init(UART0, &UART_initStruct);
	
	IRQ_Connect(IRQ0_15_UART0, IRQ0_IRQ, 1);
	
	UART_Open(UART0);
}

static void uart0_configuration(uint32_t bund)
{
    uart0_init(bund);
}


static int uart0_write(cola_device_t *dev, int pos, const void *buffer, int size)
{
    int i;
    uint8_t *buf = (uint8_t *)buffer;
    for(i=0;i<size;i++)
    {
        UART_WriteByte(UART0, buf[i]);

        while(UART_IsTXBusy(UART0));
    }
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
        uart0_init(cfg->baud_rate);
    }
    return 0;
}


void IRQ0_Handler(void)
{
	uint32_t chr;
	uint8_t c;
	if(UART_INTRXThresholdStat(UART0) || UART_INTTimeoutStat(UART0))
	{
		while(UART_IsRXFIFOEmpty(UART0) == 0)
		{
			if(UART_ReadByte(UART0, &chr) == 0)
			{
                if(uart0_dev.owner)
                {
                    c = (uint8_t)chr;
                    uart0_write_slot( &c, 1);
                }
			}
		}
        cola_timer_start(&uart0_timer,TIMER_ONE_SHOT,pdMS_TO_TICKS(20));
	}
}



static struct cola_device_ops uart0_ops =
{
	.write  = uart0_write,
    .read   = uart0_read,
    .config = uart0_config,
};

static void uart_timer_cb(void *args,uint32_t event)
{
    if(uart0_dev.owner)
    {
        cola_set_event(uart0_dev.owner,SIG_DATA);
    }
}
#endif

void board_setup_usart(void)
{
#ifdef USING_UART0
    uart0_configuration(115200);
    cola_fifo_init(&uart0_fifo,uart0_buf,sizeof(uart0_buf));
    cola_timer_create(&uart0_timer,uart_timer_cb,NULL);
    uart0_dev.name = "uart1";
    uart0_dev.dops = &uart0_ops;
    cola_device_register(&uart0_dev);
#endif

}

fs_initcall(board_setup_usart);
