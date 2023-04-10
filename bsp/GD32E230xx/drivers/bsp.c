#include "config.h"
#include "bsp.h"
#include "cola_init.h"
#include "cola_os.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


/*!
    \brief      configure systick
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void systick_config(void)
{
    /* setup systick timer for 1000Hz interrupts */
    if (SysTick_Config(SystemCoreClock / 1000U)){
        /* capture error */
        while (1){
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
}

/*!
    \brief      configure the different system clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
  /* enable GPIOC clock */
  rcu_periph_clock_enable(RCU_GPIOA);
  /* enable ADC clock */
  rcu_periph_clock_enable(RCU_ADC);
  /* enable DMA clock */
  rcu_periph_clock_enable(RCU_DMA);
  /* config ADC clock */
  rcu_adc_clock_config(RCU_ADCCK_APB2_DIV6);//
}

void sys_tick_init(void)
{
    systick_config();
}

void SysTick_Handler(void)
{
    cola_timer_ticker();
}

#define USING_HSI

void bsp_rcc(void)
{
    sys_tick_init();
    rcu_config();
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



