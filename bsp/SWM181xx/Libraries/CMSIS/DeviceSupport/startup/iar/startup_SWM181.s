;******************************************************************************************************************************************
; 文件名称:    startup_SWM181.s
; 功能说明:    SWM181单片机的启动文件
; 技术支持:    http://www.synwit.com.cn/e/tool/gbook/?bid=1
; 注意事项:
; 版本日期: V1.0.0        2016年1月30日
; 升级记录:
;
;
;******************************************************************************************************************************************
; @attention
;
; THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
; REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE
; FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
; OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
; -ECTION WITH THEIR PRODUCTS.
;
; COPYRIGHT 2012 Synwit Technology
;******************************************************************************************************************************************


; Sram Switch Configuration
;             Code Size         Sram Size
;    0        16k                16k                Cache使能，程序存储器240K
;    1        8k                24k                Cache使能，程序存储器240K
;    3        24k                8k

SRAM_SWITCH      EQU    3;


        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        PUBLIC  __vector_table

        DATA
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler
        DCD     NMI_Handler
        DCD     HardFault_Handler
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     0x0B11FFAC
        DCD     0x6000
        DCD     SRAM_SWITCH
        DCD     SVC_Handler               ; SVCall Handler
        DCD     0
        DCD     0
        DCD     PendSV_Handler            ; PendSV Handler
        DCD     SysTick_Handler           ; SysTick Handler

        ; External Interrupts
        DCD     IRQ0_Handler
        DCD     IRQ1_Handler
        DCD     IRQ2_Handler
        DCD     IRQ3_Handler
        DCD     IRQ4_Handler
        DCD     IRQ5_Handler
        DCD     IRQ6_Handler
        DCD     IRQ7_Handler
        DCD     IRQ8_Handler
        DCD     IRQ9_Handler
        DCD     IRQ10_Handler
        DCD     IRQ11_Handler
        DCD     IRQ12_Handler
        DCD     IRQ13_Handler
        DCD     IRQ14_Handler
        DCD     IRQ15_Handler
        DCD     IRQ16_Handler
        DCD     IRQ17_Handler
        DCD     IRQ18_Handler
        DCD     IRQ19_Handler
        DCD     IRQ20_Handler
        DCD     IRQ21_Handler
        DCD     IRQ22_Handler
        DCD     IRQ23_Handler
        DCD     IRQ24_Handler
        DCD     IRQ25_Handler
        DCD     IRQ26_Handler
        DCD     IRQ27_Handler
        DCD     IRQ28_Handler
        DCD     IRQ29_Handler
        DCD     IRQ30_Handler
        DCD     IRQ31_Handler



        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        LDR     R0, =__iar_program_start
        BX      R0
        
        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B SysTick_Handler


        PUBWEAK IRQ0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ0_Handler
        B IRQ0_Handler

        PUBWEAK IRQ1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ1_Handler
        B IRQ1_Handler

        PUBWEAK IRQ2_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ2_Handler
        B IRQ2_Handler

        PUBWEAK IRQ3_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ3_Handler
        B IRQ3_Handler

        PUBWEAK IRQ4_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ4_Handler
        B IRQ4_Handler

        PUBWEAK IRQ5_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ5_Handler
        B IRQ5_Handler

        PUBWEAK IRQ6_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ6_Handler
        B IRQ6_Handler

        PUBWEAK IRQ7_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ7_Handler
        B IRQ7_Handler

        PUBWEAK IRQ8_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ8_Handler
        B IRQ8_Handler

        PUBWEAK IRQ9_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ9_Handler
        B IRQ9_Handler

        PUBWEAK IRQ10_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ10_Handler
        B IRQ10_Handler

        PUBWEAK IRQ11_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ11_Handler
        B IRQ11_Handler

        PUBWEAK IRQ12_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ12_Handler
        B IRQ12_Handler

        PUBWEAK IRQ13_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ13_Handler
        B IRQ13_Handler

        PUBWEAK IRQ14_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ14_Handler
        B IRQ14_Handler

        PUBWEAK IRQ15_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ15_Handler
        B IRQ15_Handler

        PUBWEAK IRQ16_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ16_Handler
        B IRQ16_Handler

        PUBWEAK IRQ17_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ17_Handler
        B IRQ17_Handler

        PUBWEAK IRQ18_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ18_Handler
        B IRQ18_Handler

        PUBWEAK IRQ19_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ19_Handler
        B IRQ19_Handler

        PUBWEAK IRQ20_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ20_Handler
        B IRQ20_Handler

        PUBWEAK IRQ21_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ21_Handler
        B IRQ21_Handler

        PUBWEAK IRQ22_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ22_Handler
        B IRQ22_Handler

        PUBWEAK IRQ23_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ23_Handler
        B IRQ23_Handler

        PUBWEAK IRQ24_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ24_Handler
        B IRQ24_Handler

        PUBWEAK IRQ25_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ25_Handler
        B IRQ25_Handler

        PUBWEAK IRQ26_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ26_Handler
        B IRQ26_Handler

        PUBWEAK IRQ27_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ27_Handler
        B IRQ27_Handler

        PUBWEAK IRQ28_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ28_Handler
        B IRQ28_Handler

        PUBWEAK IRQ29_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ29_Handler
        B IRQ29_Handler

        PUBWEAK IRQ30_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ30_Handler
        B IRQ30_Handler

        PUBWEAK IRQ31_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ31_Handler
        B IRQ31_Handler


        END
