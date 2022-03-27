/**
  ********************************  STM32F0xx  *********************************
  * @文件名     ： bsp.c
  * @作者       ： sun
  * @库版本     ： V1.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2016年05月28日
  * @摘要       ： BSP板级支持包源文件
  ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
  2016-05-28 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
/* 包含的头文件 --------------------------------------------------------------*/
#include "config.h"
#include "bsp.h"
#include "cola_init.h"
#include "cola_os.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

void sys_tick_init(void)
{
    RCC_ClocksTypeDef RCC_Clocks;
    RCC_GetClocksFreq(&RCC_Clocks);
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);  
    SysTick_Config(RCC_Clocks.HCLK_Frequency /(configHZ));
    NVIC_SetPriority (SysTick_IRQn, 0);  
}

void SysTick_Handler(void)
{
    cola_timer_ticker();
}

#define USING_HSI

void bsp_rcc(void)
{
    
    RCC_DeInit(); 
#ifdef USING_HSI
     /* Set HSION bit */
    RCC->CR |= (uint32_t)RCC_CR_HSION;

    while ((RCC->CR & RCC_CR_HSIRDY) == RESET);
    
    /* Enable Prefetch Buffer and set Flash Latency */
    FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;

    /* HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
    /* PCLK = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE_DIV1;
    
    /* PLL configuration = HSI / 2 * 12 = 48 MHz */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSI_DIV2 | RCC_CFGR_PLLMULL12);

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == RESET);

    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t) ~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL);
	
#else 
/* SYSCLK, HCLK, PCLK configuration ----------------------------------------*/
    /* Enable HSE */
    RCC->CR |= ((uint32_t)RCC_CR_HSEON);

    while ((RCC->CR & RCC_CR_HSERDY) == RESET);

    /* Enable Prefetch Buffer and set Flash Latency */
    FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;

    /* HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

    /* PCLK = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE_DIV1;

    /* PLL configuration = HSE * 6 = 48 MHz */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_PREDIV1 | RCC_CFGR_PLLXTPRE_PREDIV1 | RCC_CFGR_PLLMULL6);

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == RESET);

    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL);
#endif
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    sys_tick_init();
}
pure_initcall(bsp_rcc);

void cola_show_version(void)
{    
    os_log("\n \\ | /\n");    
    os_log("- cola -     Ver:%d.%d.%d\n",COLA_VERSION, COLA_SUBVERSION, COLA_REVISION);    
    os_log(" / | \\ \n");    
}


void bsp_init(void)
{
    do_init_call();
    
}
void bsp_app_init(void)
{
    do_app_init_call();
    cola_show_version();
}

void bsp_restart(void)
{
     NVIC_SystemReset();
}
uint32_t bsp_get_radom(uint32_t range_min,uint32_t range_max)
{
    uint32_t seed = jiffies;
    if(range_max==range_min)
    {
        return range_min;
    }
    if(range_min > range_max)
    {
        return 0;
    }
    srand((unsigned)seed);
    return rand() % (range_max-range_min) + range_min;
}



