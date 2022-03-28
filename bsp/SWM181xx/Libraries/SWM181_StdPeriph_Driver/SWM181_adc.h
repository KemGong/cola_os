#ifndef __SWM181_ADC_H__
#define	__SWM181_ADC_H__

typedef struct {
	uint8_t clk_src;		//ADCת��ʱ��Դ��ADC_CLKSRC_HRC_DIV4��ADC_CLKSRC_XTAL_DIV8
	uint8_t channels;		//ADCת��ͨ��ѡ�У�ADC_CH0��ADC_CH1��... ... ��ADC_CH7������ϣ�������λ�����㣩
	uint8_t samplAvg;		//����ȡƽ������������ADCת����ADC��һ��ͨ��������������ת����Σ��������ǵ�ƽ��ֵ��Ϊ��ͨ��ת�����
	uint8_t trig_src;		//ADC������ʽ��ADC_TRIGSRC_SW��ADC_TRIGSRC_PWM��ADC_TRIGSRC_TIMR2��ADC_TRIGSRC_TIMR3
	uint8_t Continue;		//���������ģʽ�£�1 ����ת��ģʽ��������һֱ������ת����ֱ��������STARTλ
							//                  0 ����ת��ģʽ��ת����ɺ�STARTλ�Զ����ֹͣת��
	uint8_t EOC_IEn;		//EOC�ж�ʹ�ܣ������ÿ��ͨ�����ã�����ЧֵΪADC_CH0��ADC_CH1��... ... ��ADC_CH7������ϣ�������λ�����㣩
	uint8_t OVF_IEn;		//OVF�ж�ʹ�ܣ������ÿ��ͨ�����ã�����ЧֵΪADC_CH0��ADC_CH1��... ... ��ADC_CH7������ϣ�������λ�����㣩
} ADC_InitStructure;

#define ADC_CH0		0x01
#define ADC_CH1		0x02
#define ADC_CH2		0x04
#define ADC_CH3		0x08
#define ADC_CH4		0x10
#define ADC_CH5		0x20
#define ADC_CH6		0x40
#define ADC_CH7		0x80

#define ADC_CLKSRC_HRC_DIV4		0	//�ڲ���ƵRC����4��Ƶ
#define ADC_CLKSRC_XTAL_DIV8	1	//�ⲿ��������8��Ƶ

#define ADC_AVG_SAMPLE1			0	
#define ADC_AVG_SAMPLE2			1	//һ����������������ת��2�Σ����������ν����ƽ��ֵ��Ϊת�����
#define ADC_AVG_SAMPLE4			3
#define ADC_AVG_SAMPLE8			7
#define ADC_AVG_SAMPLE16		15

#define ADC_TRIGSRC_SW			0	//�����������ADC->START.GOд1����ת��
#define ADC_TRIGSRC_PWM			1
#define ADC_TRIGSRC_TIMR2		2
#define ADC_TRIGSRC_TIMR3		3


void ADC_Init(ADC_TypeDef * ADCx, ADC_InitStructure * initStruct);		//ADCģ��ת������ʼ��
void ADC_Open(ADC_TypeDef * ADCx);							//ADC���������������������Ӳ������ADCת��
void ADC_Close(ADC_TypeDef * ADCx);							//ADC�رգ��޷������������Ӳ������ADCת��
void ADC_Start(ADC_TypeDef * ADCx);							//����ָ��ADC����ʼģ��ת��
void ADC_Stop(ADC_TypeDef * ADCx);							//�ر�ָ��ADC��ֹͣģ��ת��

uint32_t ADC_Read(ADC_TypeDef * ADCx, uint32_t chn);		//��ָ��ͨ����ȡת�����
uint32_t ADC_IsEOC(ADC_TypeDef * ADCx, uint32_t chn);		//ָ��ͨ���Ƿ�End Of Conversion

void ADC_ChnSelect(ADC_TypeDef * ADCx, uint32_t chns);


void ADC_IntEOCEn(ADC_TypeDef * ADCx, uint32_t chn);		//ת������ж�ʹ��
void ADC_IntEOCDis(ADC_TypeDef * ADCx, uint32_t chn);		//ת������жϽ�ֹ
void ADC_IntEOCClr(ADC_TypeDef * ADCx, uint32_t chn);		//ת������жϱ�־���
uint32_t ADC_IntEOCStat(ADC_TypeDef * ADCx, uint32_t chn);	//ת������ж�״̬
void ADC_IntOVFEn(ADC_TypeDef * ADCx, uint32_t chn);		//��������ж�ʹ��
void ADC_IntOVFDis(ADC_TypeDef * ADCx, uint32_t chn);		//��������жϽ�ֹ
void ADC_IntOVFClr(ADC_TypeDef * ADCx, uint32_t chn);		//��������жϱ�־���
uint32_t ADC_IntOVFStat(ADC_TypeDef * ADCx, uint32_t chn);	//��������ж�״̬



#endif //__SWM181_ADC_H__
