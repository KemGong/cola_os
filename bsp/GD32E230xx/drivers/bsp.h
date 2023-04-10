/**
  *********************************  STM32F0xx  ********************************
  * @�ļ���     �� bsp.h
  * @����       �� sun
  * @��汾     �� V1.5.0
  * @�ļ��汾   �� V1.0.0
  * @����       �� 2016��05��28��
  * @ժҪ       �� BSP�弶֧�ְ�ͷ�ļ�
  ******************************************************************************/

/* �����ֹ�ݹ���� ----------------------------------------------------------*/
#ifndef _BSP_H
#define _BSP_H

/* ������ͷ�ļ� --------------------------------------------------------------*/

#include <stdint.h>
#include "gd32e23x_libopt.h"


#define    disable_irq()  __disable_irq()
#define    enable_irq()    __enable_irq()    

#define OS_CPU_SR   uint32_t
#define enter_critical()        \
    do { cpu_sr = __get_PRIMASK(); __disable_irq();} while (0)
#define exit_critical()         \
    do { __set_PRIMASK(cpu_sr);} while (0)



uint32_t bsp_get_radom(uint32_t range_min,uint32_t range_max);

void bsp_init(void);
void bsp_app_init(void);
void bsp_restart(void);
#endif /* _BSP_H */

/**** Copyright (C)2016 sun. All Rights Reserved **** END OF FILE ****/
