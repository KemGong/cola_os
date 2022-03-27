#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "bsp.h"
#include "cola_os.h"
#include "SWM181.h"
#include "cola_init.h"


void SysTick_Handler(void)
{
    cola_timer_ticker();
}
static void sys_tick_init(void)
{
    SysTick_Config(SystemCoreClock/configHZ);	
}

static void bsp_rcc(void)
{
    SystemInit();
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


