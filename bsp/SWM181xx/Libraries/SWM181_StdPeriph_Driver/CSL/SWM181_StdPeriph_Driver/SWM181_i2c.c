/****************************************************************************************************************************************** 
* �ļ�����:	SWM181_i2c.c
* ����˵��:	SWM181��Ƭ����I2C���нӿڹ���������
* ����֧��:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
* ע������:
* �汾����:	V1.0.0		2016��1��30��
* ������¼:  
*
*
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
#include "SWM181_i2c.h"


/****************************************************************************************************************************************** 
* ��������:	I2C_Init()
* ����˵��:	I2C��ʼ��
* ��    ��: I2C_TypeDef * I2Cx		ָ��Ҫ�����õ�I2C����Чֵ����I2C0��I2C1
*			I2C_InitStructure * initStruct	����I2C����趨ֵ�Ľṹ��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void I2C_Init(I2C_TypeDef * I2Cx, I2C_InitStructure * initStruct)
{
	switch((uint32_t)I2Cx)
	{
	case ((uint32_t)I2C0):
		SYS->CLKEN |= (0x01 << SYS_CLKEN_I2C0_Pos);
		break;
	
	case ((uint32_t)I2C1):
		SYS->CLKEN |= (0x01 << SYS_CLKEN_I2C1_Pos);
		break;
	}
	
	I2C_Close(I2Cx);	//һЩ�ؼ��Ĵ���ֻ����I2C�ر�ʱ����
	
	if(initStruct->Master == 1)
	{
		I2Cx->SLVCR &= ~I2C_SLVCR_SLAVE_Msk;
		I2Cx->SLVCR |= (0 << I2C_SLVCR_SLAVE_Pos);
		
		I2Cx->CLKDIV = SystemCoreClock/5/initStruct->MstClk - 1;
		
		I2Cx->MSTCMD = (I2Cx->MSTCMD & (~I2C_MSTCMD_IF_Msk)) | (1 << I2C_MSTCMD_IF_Pos);	//ʹ���ж�֮ǰ������жϱ�־
		I2Cx->CTRL &= ~I2C_CTRL_MSTIE_Msk;
		I2Cx->CTRL |= (initStruct->MstIEn << I2C_CTRL_MSTIE_Pos);
	}
	else
	{
		I2Cx->SLVCR &= ~I2C_SLVCR_SLAVE_Msk;
		I2Cx->SLVCR |= (1 << I2C_SLVCR_SLAVE_Pos);
		
		I2Cx->SLVCR &= ~(I2C_SLVCR_ADDR7b_Msk | I2C_SLVCR_ADDR_Msk);
		I2Cx->SLVCR |= (1 << I2C_SLVCR_ACK_Pos) |
					   (initStruct->Addr7b << I2C_SLVCR_ADDR7b_Pos) |
					   (initStruct->SlvAddr << I2C_SLVCR_ADDR_Pos);
		I2Cx->SLVAM  = initStruct->SlvAddrMask;
		
		I2Cx->SLVIF = I2C_SLVIF_RXEND_Msk | I2C_SLVIF_TXEND_Msk | I2C_SLVIF_STADET_Msk | I2C_SLVIF_STODET_Msk;	//���жϱ�־
		I2Cx->SLVCR &= ~(I2C_SLVCR_IM_RXEND_Msk | I2C_SLVCR_IM_TXEND_Msk | I2C_SLVCR_IM_STADET_Msk | I2C_SLVCR_IM_STODET_Msk |
						 I2C_SLVCR_IM_RDREQ_Msk | I2C_SLVCR_IM_WRREQ_Msk);
		I2Cx->SLVCR |= ((initStruct->SlvRxEndIEn  ? 0 : 1) << I2C_SLVCR_IM_RXEND_Pos)  |
					   ((initStruct->SlvTxEndIEn  ? 0 : 1) << I2C_SLVCR_IM_TXEND_Pos)  |
					   ((initStruct->SlvSTADetIEn ? 0 : 1) << I2C_SLVCR_IM_STADET_Pos) |
					   ((initStruct->SlvSTODetIEn ? 0 : 1) << I2C_SLVCR_IM_STODET_Pos) |
					   ((initStruct->SlvRdReqIEn  ? 0 : 1) << I2C_SLVCR_IM_RDREQ_Pos)  |
					   ((initStruct->SlvWrReqIEn  ? 0 : 1) << I2C_SLVCR_IM_WRREQ_Pos);
	}
}

/****************************************************************************************************************************************** 
* ��������:	I2C_Open()
* ����˵��:	I2C�򿪣������շ�
* ��    ��: I2C_TypeDef * I2Cx		ָ��Ҫ�����õ�I2C����Чֵ����I2C0��I2C1
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void I2C_Open(I2C_TypeDef * I2Cx)
{
	I2Cx->CTRL |= (0x01 << I2C_CTRL_EN_Pos);
}

/****************************************************************************************************************************************** 
* ��������:	I2C_Close()
* ����˵��:	I2C�رգ���ֹ�շ�
* ��    ��: I2C_TypeDef * I2Cx		ָ��Ҫ�����õ�I2C����Чֵ����I2C0��I2C1
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void I2C_Close(I2C_TypeDef * I2Cx)
{
	I2Cx->CTRL &= ~I2C_CTRL_EN_Msk;
}

/****************************************************************************************************************************************** 
* ��������:	I2C_Start()
* ����˵��:	������ʼ�źŲ������豸��ַ
* ��    ��: I2C_TypeDef * I2Cx		ָ��Ҫ�����õ�I2C����Чֵ����I2C0��I2C1
*			uint8_t addr			�豸��ַ
* ��    ��: uint8_t					1 ���յ�ACK   0 ���յ�NACK
* ע������: ��
******************************************************************************************************************************************/
uint8_t I2C_Start(I2C_TypeDef * I2Cx, uint8_t addr)
{
	I2Cx->MSTDAT = addr;
	I2Cx->MSTCMD = (1 << I2C_MSTCMD_STA_Pos) |		
				   (1 << I2C_MSTCMD_WR_Pos);				//������ʼλ�ʹӻ���ַ
	while(I2Cx->MSTCMD & I2C_MSTCMD_TIP_Msk) __NOP();		//�ȴ��������
	
	return (I2Cx->MSTCMD & I2C_MSTCMD_RXACK_Msk) ? 0 : 1;
}

/****************************************************************************************************************************************** 
* ��������:	I2C_Stop()
* ����˵��:	����ֹͣ�ź�
* ��    ��: I2C_TypeDef * I2Cx		ָ��Ҫ�����õ�I2C����Чֵ����I2C0��I2C1
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void I2C_Stop(I2C_TypeDef * I2Cx)
{
	I2Cx->MSTCMD = (1 << I2C_MSTCMD_STO_Pos);
	while(I2Cx->MSTCMD & I2C_MSTCMD_TIP_Msk) __NOP();		//�ȴ��������
}

/****************************************************************************************************************************************** 
* ��������:	I2C_Write()
* ����˵��:	д��һ������
* ��    ��: I2C_TypeDef * I2Cx		ָ��Ҫ�����õ�I2C����Чֵ����I2C0��I2C1
*			uint8_t data			Ҫд������
* ��    ��: uint8_t					1 ���յ�ACK   0 ���յ�NACK
* ע������: ��
******************************************************************************************************************************************/
uint8_t I2C_Write(I2C_TypeDef * I2Cx, uint8_t data)
{	
	I2Cx->MSTDAT = data;
	I2Cx->MSTCMD = (1 << I2C_MSTCMD_WR_Pos);
	while(I2Cx->MSTCMD & I2C_MSTCMD_TIP_Msk) __NOP();		//�ȴ��������
	
	return (I2Cx->MSTCMD & I2C_MSTCMD_RXACK_Msk) ? 0 : 1;
}

/****************************************************************************************************************************************** 
* ��������:	I2C_Read()
* ����˵��:	��ȡһ������
* ��    ��: I2C_TypeDef * I2Cx		ָ��Ҫ�����õ�I2C����Чֵ����I2C0��I2C1
*			uint8_t ack				1 ����ACK   0 ����NACK
* ��    ��: uint8_t					��ȡ��������
* ע������: ��
******************************************************************************************************************************************/
uint8_t I2C_Read(I2C_TypeDef * I2Cx, uint8_t ack)
{
	I2Cx->MSTCMD = (1 << I2C_MSTCMD_RD_Pos) |
				   ((ack ? 0 : 1) << I2C_MSTCMD_ACK_Pos);
	while(I2Cx->MSTCMD & I2C_MSTCMD_TIP_Msk) __NOP();		//�ȴ��������
    
    return I2Cx->MSTDAT;
}


/****************************************************************************************************************************************** 
* ��������:	I2C_INTEn()
* ����˵��:	ʹ���ж�
* ��    ��: I2C_TypeDef * I2Cx	ָ��Ҫ�����õ�I2C����ȡֵ����I2C0��I2C1
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void I2C_INTEn(I2C_TypeDef * I2Cx)
{
	I2Cx->CTRL |= (1 << I2C_CTRL_MSTIE_Pos);
}

/****************************************************************************************************************************************** 
* ��������: I2C_INTDis()
* ����˵��:	�����ж�
* ��    ��: I2C_TypeDef * I2Cx	ָ��Ҫ�����õ�I2C����ȡֵ����I2C0��I2C1
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void I2C_INTDis(I2C_TypeDef * I2Cx)
{
	I2Cx->CTRL &= ~(1 << I2C_CTRL_MSTIE_Pos);
}

/****************************************************************************************************************************************** 
* ��������:	I2C_INTClr()
* ����˵��:	����жϱ�־
* ��    ��: I2C_TypeDef * I2Cx	ָ��Ҫ�����õ�I2C����ȡֵ����I2C0��I2C1
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void I2C_INTClr(I2C_TypeDef * I2Cx)
{
	I2Cx->MSTCMD |= (1 << I2C_MSTCMD_IF_Pos);
}

/****************************************************************************************************************************************** 
* ��������: I2C_INTStat()
* ����˵��:	��ȡ�ж�״̬
* ��    ��: I2C_TypeDef * I2Cx	ָ��Ҫ�����õ�I2C����ȡֵ����I2C0��I2C1
* ��    ��: uint32_t 				0 I2Cxδ�����ж�    1 I2Cx�������ж�
* ע������: ��
******************************************************************************************************************************************/
uint32_t I2C_INTStat(I2C_TypeDef * I2Cx)
{
	return (I2Cx->MSTCMD & I2C_MSTCMD_IF_Msk) ? 1 : 0;
}
