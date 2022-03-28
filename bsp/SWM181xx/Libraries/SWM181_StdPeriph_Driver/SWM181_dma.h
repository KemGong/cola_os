#ifndef __SWM181_DMA_H__
#define __SWM181_DMA_H__


#define DMA_CHW_FLASH	0	//дFlashͨ��
#define DMA_CHR_FLASH	1	//��Flashͨ��
#define DMA_CHR_ADC		2	//��ADCͨ��
#define DMA_CHR_SDADC	4	//��SDADCͨ��
#define DMA_CHR_CAN		5	//��CANͨ��


void DMA_CH_Config(uint32_t chn, uint32_t ram_addr, uint32_t num_word, uint32_t int_en);	//DMAͨ������
void DMA_CHM_Config(uint32_t chn, uint32_t src_addr, uint32_t dst_addr, uint32_t num_word, uint32_t int_en);	//DMAͨ�����ã����ڴ洢���䣨��Flash��RAM�䣩��������
void DMA_CH_Open(uint32_t chn);					//DMAͨ����
void DMA_CH_Close(uint32_t chn);				//DMAͨ���ر�

void DMA_CH_INTEn(uint32_t chn);				//DMA�ж�ʹ�ܣ����ݰ�����ɺ󴥷��ж�
void DMA_CH_INTDis(uint32_t chn);				//DMA�жϽ�ֹ�����ݰ�����ɺ󲻴����ж�
void DMA_CH_INTClr(uint32_t chn);				//DMA�жϱ�־���
uint32_t DMA_CH_INTStat(uint32_t chn);			//DMA�ж�״̬��ѯ��1 ���ݰ������    0 ���ݰ���δ���



#endif //__SWM181_DMA_H__
