/**
  *********************************  STM32F0xx  ********************************
  * @文件名     ： bsp.h
  * @作者       ： sun
  * @库版本     ： V1.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2016年05月28日
  * @摘要       ： BSP板级支持包头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _BSP_H
#define _BSP_H

/* 包含的头文件 --------------------------------------------------------------*/

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
