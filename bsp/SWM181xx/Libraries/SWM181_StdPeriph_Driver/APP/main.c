#include "SWM181.h"

#include <math.h>


void SerialInit(void);

int main(void)
{
 	uint32_t i;
 	
 	SystemInit();
	
	SerialInit();
	
	GPIO_Init(GPIOA, PIN4, 1, 0, 0, 0);			//GPIOA.4����Ϊ������ţ��������
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);			//GPIOA.5����Ϊ������ţ��������
	GPIO_ClrBit(GPIOA, PIN4);
 	GPIO_SetBit(GPIOA, PIN5);
    
 	while(1==1)
 	{
 		GPIO_InvBit(GPIOA, PIN4);
 		GPIO_InvBit(GPIOA, PIN5);
 		for(i=0; i<5000000; i++) i=i;
		
  		printf("Hi, World!\r\n");
 	}
}

void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTA, PIN0, FUNMUX_UART0_RXD, 1);	//GPIOA.0����ΪUART0��������
	PORT_Init(PORTA, PIN1, FUNMUX_UART0_TXD, 0);	//GPIOA.1����ΪUART0�������
 	
 	UART_initStruct.Baudrate = 57600;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThreshold = 3;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThreshold = 3;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutTime = 10;
	UART_initStruct.TimeoutIEn = 0;
 	UART_Init(UART0, &UART_initStruct);
	UART_Open(UART0);
}

/****************************************************************************************************************************************** 
* ��������: fputc()
* ����˵��: printf()ʹ�ô˺������ʵ�ʵĴ��ڴ�ӡ����
* ��    ��: int ch		Ҫ��ӡ���ַ�
*			FILE *f		�ļ����
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}
