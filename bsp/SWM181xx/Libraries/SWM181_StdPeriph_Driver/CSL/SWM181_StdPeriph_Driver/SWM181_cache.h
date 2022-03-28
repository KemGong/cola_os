#ifndef __SWM181_CACHE_H__
#define __SWM181_CACHE_H__

typedef struct {
	uint8_t  Alg;			//Cache�㷨����ȡֵCACHE_ALG_LRU��CACHE_ALG_RAND
	
	uint8_t  Threshold;		//����LOCK��SLOT��������Thresholdʱ����Reset�ж�
	
	uint8_t  ResetIEn;		//��LOCKCNT > LOCKTHRʱ����Reset�жϣ��жϴ�������Ҫִ��CACHE->CR.RST = 1��λCACHE
} CACHE_InitStructure;

#define CACHE_ALG_LRU	0
#define CACHE_ALG_RAND	1


void CACHE_Init(CACHE_InitStructure * initStruct);

void CACHE_Reset(void);

uint32_t CACHE_Prefetch(uint32_t addr);
uint32_t CACHE_PrefetchSlotNumber(void);

uint32_t CACHE_Invalid(uint32_t addr);
uint32_t CACHE_InvalidSlotNumber(void);


#endif //__SWM181_CACHE_H__
