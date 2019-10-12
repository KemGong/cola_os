#include "config.h"
#include "stm32l0xx_hal.h"
#include "stm32_sleep.h"
#include "cola_os.h"
#include "cola_device.h"
#include "rtc.h"
#include "bsp.h"

static cola_device_t sleep_dev;


void board_sleep_init(void)
{
    MX_RTC_Init();
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();
    HAL_PWR_DisablePVD();
    HAL_PWREx_EnableUltraLowPower();
    HAL_PWREx_EnableFastWakeUp();
    __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_HSI);

}


void board_exit_sleep(void)
{
    if(__HAL_PWR_GET_FLAG(PWR_FLAG_WU) != RESET)
	{
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);	
	}
	if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
	{
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB); 
	}
    bsp_rcc();
    __GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();
}


void board_enter_sleep(uint16_t time)
{

    __HAL_RCC_GPIOA_CLK_DISABLE();
    __HAL_RCC_GPIOB_CLK_DISABLE();
    __HAL_RCC_GPIOC_CLK_DISABLE();
    __HAL_RCC_GPIOH_CLK_DISABLE();


    //HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
    // 若需要在低功耗模式下调试程序，可以打开此函数
    HAL_DBGMCU_DisableDBGStopMode();
    //HAL_DBGMCU_DBG_DisableLowPowerConfig(DBGMCU_SLEEP | DBGMCU_STOP | DBGMCU_STANDBY);

    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);	// 清除唤醒标志
    //HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, time, RTC_WAKEUPCLOCK_RTCCLK_DIV16);
    HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, time-1, RTC_WAKEUPCLOCK_CK_SPRE_16BITS); //wangxl@20190814 ?时钟选择1Hz ?65535/60/60 ~18 ?可得到的唤醒时间为 1s 到 18h 左右
    //__HAL_RCC_WAKEUPSTOP_CLK_CONFIG (RCC_STOP_WAKEUPCLOCK_HSI);
    /* Enter Stop Mode */
   // __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
    board_exit_sleep();
      
}
void board_enter_deep_sleep(uint16_t time)
{

	__HAL_RCC_GPIOA_CLK_DISABLE();
	__HAL_RCC_GPIOB_CLK_DISABLE();
	__HAL_RCC_GPIOC_CLK_DISABLE();
	__HAL_RCC_GPIOH_CLK_DISABLE();


	HAL_PWREx_EnableUltraLowPower();   //使能超低功耗

	HAL_PWREx_EnableFastWakeUp();      //使能快速唤醒
	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
	// 若需要在低功耗模式下调试程序，可以打开此函数
	HAL_DBGMCU_DBG_DisableLowPowerConfig(DBGMCU_SLEEP | DBGMCU_STOP | DBGMCU_STANDBY);
    
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);	// 清除唤醒标志
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);	// 清除待机标志

	//HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, time, RTC_WAKEUPCLOCK_RTCCLK_DIV16);
	HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, time-1, RTC_WAKEUPCLOCK_CK_SPRE_16BITS); //wangxl@20190814 ?时钟选择1Hz ?65535/60/60 ~18 ?可得到的唤醒时间为 1s 到 18h 左右
	//__HAL_RCC_WAKEUPSTOP_CLK_CONFIG (RCC_STOP_WAKEUPCLOCK_HSI);
	/* Enter Stop Mode */
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWR_EnterSTANDBYMode();
      
}
static int sleep_ctrl(cola_device_t *dev, int cmd, void *args)
{
   if(DEV_SLEEP == cmd)
   {
        uint16_t time = *((uint16_t *)args);
        board_enter_sleep(time);
   }
   else if(DEV_DEEP_SLEEP == cmd)
   {
        uint16_t time = *((uint16_t *)args);
        board_enter_deep_sleep(time);
   }
    return 0;
}
static struct cola_device_ops ops =
{
    .control = sleep_ctrl,
};

void RTC_IRQHandler(void)
{
	/* USER CODE BEGIN RTC_IRQn 0 */

	/* USER CODE END RTC_IRQn 0 */
	HAL_RTCEx_WakeUpTimerIRQHandler(&hrtc);
	/* USER CODE BEGIN RTC_IRQn 1 */

	/* USER CODE END RTC_IRQn 1 */
}

void board_setup_sleep(void)
{
    board_sleep_init();
    sleep_dev.name = "sleep";
    sleep_dev.dops = &ops;
    cola_device_register(&sleep_dev);

}
device_initcall(board_setup_sleep);
