
#include "stm32f0xx.h"
#include "systick.h"
#include "cola_os.h"


void sys_tick_init(uint16_t ms, uint8_t priority)
{
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.SYSCLK_Frequency /(1000/ms));
  NVIC_SetPriority (SysTick_IRQn, priority);  
	
}

void SysTick_Handler(void)
{
    cola_timer_ticker();
}


