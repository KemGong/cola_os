/****************************************************************************************************************************************** 
* 文件名称:	SWM181_flash.c
* 功能说明:	使用芯片的IAP功能将片上Flash模拟成EEPROM来保存数据，掉电后不丢失
* 技术支持:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
* 注意事项:
* 版本日期: V1.0.0		2016年1月30日
* 升级记录: 
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
#include "SWM181_flash.h"

typedef void (*IAPFunc)(uint32_t faddr, uint32_t raddr, uint32_t cnt, uint32_t cmd);
IAPFunc IAPfunc = (IAPFunc)0x1000601;

/****************************************************************************************************************************************** 
* 函数名称:	FLASH_Erase()
* 功能说明:	FLASH扇区擦除，每个扇区4K字节
* 输    入: uint32_t addr		要擦除扇区的地址，必须4K对齐，即addr%4096 == 0
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void FLASH_Erase(uint32_t addr)
{
	uint32_t i;
	
	__disable_irq();
	
	IAPfunc(addr, 0, 0, 0x51);
	
	addr = addr / 4096 * 4096;
	for(i = 0; i < 4096; i += 128)
		CACHE_Invalid(addr + i);
	
	__enable_irq();
}

/****************************************************************************************************************************************** 
* 函数名称:	FLASH_Write()
* 功能说明:	FLASH数据写入
* 输    入: uint32_t addr		数据要写入到Flash中的地址，字对齐
*			uint32_t buff[]		要写入Flash中的数据
*			uint32_t cnt		要写的数据的个数，以字为单位，最大取值64
* 输    出: 无
* 注意事项: 要写入的数据必须全部在同一页内，即addr/256 == (addr+(cnt-1)*4)/256
******************************************************************************************************************************************/
void FLASH_Write(uint32_t addr, uint32_t buff[], uint32_t cnt)
{	
	uint32_t i;
	
	__disable_irq();
	
	FLASH->CR = (1 << FLASH_CR_FFCLR_Pos);			//Clear FIFO
	FLASH->CR = 0;
	
	IAPfunc(addr, (uint32_t)buff, cnt*4, 0x52);
	
	for(i = 0; i < cnt*4; i += 128)
		CACHE_Invalid(addr + i);
	
	__enable_irq();
}

/****************************************************************************************************************************************** 
* 函数名称:	FLASH_Read()
* 功能说明:	FLASH数据读取
* 输    入: uint32_t addr		要读取的数据在Flash中的地址，字对齐
*			uint32_t buff[]		读取到的数据存入buff指向的内存
*			uint32_t cnt		要读取的数据的个数，以字为单位
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void FLASH_Read(uint32_t addr, uint32_t buff[], uint32_t cnt)
{	
	uint32_t i;
	
	if(SYS->CACHECR & SYS_CACHECR_BYPASS_Msk)
	{
		while(FLASH->STAT & FLASH_STAT_BUSY_Msk);
		
		FLASH->CR = (1 << FLASH_CR_FFCLR_Pos);			//Clear FIFO
		
		FLASH->CR = (FLASH_CMD_READ_DATA << FLASH_CR_CMD_Pos) |
					(0 << FLASH_CR_LEN_Pos);			//连续读
		FLASH->ADDR = addr;
		FLASH->START = 1;
		
		for(i = 0; i < cnt; i++)
		{
			while(FLASH->STAT & FLASH_STAT_FE_Msk);		//FIFO Empty
			
			buff[i] = FLASH->DATA;
		}
		
		FLASH->START = 0;
		
		FLASH->CR = (1 << FLASH_CR_FFCLR_Pos);			//Clear FIFO
		FLASH->CR = 0;
	}
	else
	{
		for(i = 0; i < cnt; i++) buff[i] = *((volatile unsigned int *)(addr + i*4));
	}
}



__attribute__((section("PlaceInRAM")))
void AppToIsp(void)
{
	__disable_irq();       //这句一定要加, 防止中断打断DMA的拷贝	
	*(volatile unsigned int *)0x40000900 = 0x00; //4k mask = 0,切换到Isp区(rom code已经将该位置1)
/********************************** Set Cache And Sram Switch *******************************/ 
//	DRAM_SIZE = 0x01; //禁止cache的情况下配置sram为16/16	//之后再确认	
/********************************** Copy Code From Flash To Sram *******************************/	
	DMA->EN = 0x01;			//每个通道都有自己独立的开关控制，所以总开关可以是一直开启的				
	DMA->CH[DMA_CHR_FLASH].CR &= ~(0x01<<DMA_CR_REN_Pos);		//配置前先关闭该通道	
	DMA->CH[DMA_CHR_FLASH].SRC = 0x400;  //FLASH_ADDR
	DMA->CH[DMA_CHR_FLASH].DST = 0x00;   //SRAM_ADDR	
	DMA->CH[DMA_CHR_FLASH].CR &= ~DMA_CR_LEN_Msk;				
	DMA->CH[DMA_CHR_FLASH].CR |= ((0xC00-1)<<DMA_CR_LEN_Pos);  //copy 3K from Isp to App
	DMA->CH[DMA_CHR_FLASH].CR |= (0x01<<DMA_CR_REN_Pos);	
	while((DMA->CH[DMA_CHR_FLASH].CR&DMA_CR_REN_Msk)==0x01);		
	while(FLASH->STAT&(0x01<<16));                 //等待FlashCtrl处于空闲状态
	*(volatile unsigned int *)0x40000900 = 0x01; //4k mask = 1,切换到App区(rom code已经将该位置1)		
//	__enable_irq();	//下面执行软件复位可以不使能总中断
/********************************** System Reset(Address(0x00), Sram Code area) ****************/		
	SCB->AIRCR = ((0x5FA<<SCB_AIRCR_VECTKEY_Pos)|SCB_AIRCR_SYSRESETREQ_Msk); //系统软件复位
	__DSB();                                    /* Ensure completion of memory access */              
	while(1);		
}
