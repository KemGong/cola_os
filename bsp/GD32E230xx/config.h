/**
  ********************************************************************************
  * @filename  : cola_os.c
  * @author    : Bit (QQ 1506564994)
  * @versions  : V1.0.0
  * @date      : 2020.01.12
  * @brief     : 
  ******************************************************************************/
#ifndef _CONFIG_H
#define _CONFIG_H

#ifdef __cplusplus

extern "C"
{
  
#endif

#include <stdio.h>    
 

#define configHZ   (1000)
    
#ifndef pdMS_TO_TICKS
# define pdMS_TO_TICKS(xTimeInMs) \
    ((int)(((int)(xTimeInMs)*(int)configHZ)/(int)1000))
#endif    
   
#define USING_DEBUG
#ifdef USING_DEBUG
    #define os_log(...) printf(__VA_ARGS__)
    #define assert(p) do { \
                        if (!(p)) { \
                            os_log("BUG at assert(%s)\n", #p); \
                        }       \
                     } while (0)
#else
    #define os_log(...)
    #define assert(p) ((void)0)
   
#endif



 /********device***********/
#define USING_UART1
#define USING_OUTPUT
#define USING_ADC


#ifdef __cplusplus
}
#endif

#endif 
