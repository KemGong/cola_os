#ifndef __SYSTEM_SWM181_H__
#define __SYSTEM_SWM181_H__

#ifdef __cplusplus
 extern "C" {
#endif

	 
extern uint32_t SystemCoreClock;		// System Clock Frequency (Core Clock)
extern uint32_t CyclesPerUs;			// Cycles per micro second


extern void SystemInit(void);

extern void SystemCoreClockUpdate (void);

	 
#ifdef __cplusplus
}
#endif

#endif //__SYSTEM_SWM181_H__
