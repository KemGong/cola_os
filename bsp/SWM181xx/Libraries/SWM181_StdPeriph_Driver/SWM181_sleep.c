/****************************************************************************************************************************************** 
* �ļ�����:	SWM181_sleep.c
* ����˵��:	SWM181��Ƭ���Ľ�������ģʽ����������
* ����֧��:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
* ע������:
* �汾����: V1.0.0		2016��1��30��
* ������¼: 
*******************************************************************************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION 
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE 
* FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT 
* OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
* -ECTION WITH THEIR PRODUCTS.
*
* COPYRIGHT 2012 Synwit Technology  
*******************************************************************************************************************************************/
#include "SWM181.h"
#include "SWM181_sleep.h"


#if   defined ( __CC_ARM )

/* ��������Sleepģʽ�Ĵ���ָ�������ζ�ָ���C�����ǣ�
void EnterSleepMode(void)
{
	while((0x1<<16)&FLASH->STAT);
	SYS->SLEEP |= (1 << SYS_SLEEP_SLEEP_Pos);
}
*/
uint16_t Code_EnterSleepMode[] = {
	0x2011, 0x0680, 0x6881, 0x03C9, 0xD4FC, 0x2101, 0x0789, 0x6908, 
	0x2201, 0x4310, 0x6108, 0x4770, 
};

__asm void EnterSleepMode(void)
{
	IMPORT Code_EnterSleepMode
	PUSH {LR}
	NOP
	LDR R0,=Code_EnterSleepMode
    ADDS R0, R0, #1
	NOP
	BLX R0
	POP {R0}
	BX R0
}


/* ��������Stopģʽ�Ĵ���ָ�������ζ�ָ���C�����ǣ�
void EnterStopMode(void)
{
	while((0x1<<16)&FLASH->STAT);
	FLASH->CR = 0x1<<12;
	FLASH->START = 0x1;
	while(0x1&FLASH->START);
	SYS->SLEEP |= (1 << SYS_SLEEP_STOP_Pos);
}
*/
uint16_t Code_EnterStopMode[] = {
	0x2011, 0x0680, 0x6881, 0x03c9, 0xd4fc, 0x2101, 0x0309, 0x6001, 
	0x2101, 0x6041, 0x6841, 0x07c9, 0xd1fc, 0x2001, 0x0780, 0x6901, 
	0x2202, 0x4311, 0x6101, 0x4770,  
};

__asm void EnterStopMode(void)
{
	IMPORT Code_EnterStopMode
	PUSH {LR}
	NOP
	LDR R0,=Code_EnterStopMode
    ADDS R0, R0, #1
	NOP
	BLX R0
	POP {R0}
	BX R0
}

#elif defined ( __ICCARM__ )

__ramfunc void EnterSleepMode(void)
{
	while((0x1<<16)&FLASH->STAT);
	SYS->SLEEP |= (1 << SYS_SLEEP_SLEEP_Pos);
}

__ramfunc void EnterStopMode(void)
{
	while((0x1<<16)&FLASH->STAT);
	FLASH->CR = 0x1<<12;
	FLASH->START = 0x1;
	while(0x1&FLASH->START);
	SYS->SLEEP |= (1 << SYS_SLEEP_STOP_Pos);
}

#endif
