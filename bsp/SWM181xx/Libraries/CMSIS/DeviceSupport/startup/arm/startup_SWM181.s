;******************************************************************************************************************************************
; 文件名称:	startup_SWM181.s
; 功能说明:	SWM181单片机的启动文件
; 技术支持:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
; 注意事项:
; 版本日期: V1.0.0		2016年1月30日
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
; 			Code Size	 	Sram Size
;	0		16k				16k				Cache使能，程序存储器240K
;	1		8k				24k				Cache使能，程序存储器240K
;	3		24k		  		8k

SRAM_SWITCH	  EQU    0;


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size	  	EQU	 0x400;

				AREA	STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem	   	SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size	   	EQU	 0x000;

				AREA	HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem		SPACE   Heap_Size
__heap_limit


				PRESERVE8
				THUMB

; Vector Table Mapped to Address 0 at Reset

				AREA	RESET, DATA, READONLY
				EXPORT  __Vectors
				EXPORT  __Vectors_End
				EXPORT  __Vectors_Size

__Vectors	    DCD	 Stack_Mem + Stack_Size	; Top of Stack
				DCD	 Reset_Handler			 ; Reset Handler
				DCD	 NMI_Handler			   ; NMI Handler
				DCD	 HardFault_Handler		 ; Hard Fault Handler
				DCD	 0
				DCD	 0
				DCD	 0
				DCD	 0
				DCD	 0x0B11FFAC
				DCD	 0x6000
				DCD	 SRAM_SWITCH
				DCD	 SVC_Handler			   ; SVCall Handler
				DCD	 0
				DCD	 0
				DCD	 PendSV_Handler			; PendSV Handler
				DCD	 SysTick_Handler		   ; SysTick Handler

				; External Interrupts
				DCD	 IRQ0_Handler
				DCD	 IRQ1_Handler
				DCD	 IRQ2_Handler
				DCD	 IRQ3_Handler
				DCD	 IRQ4_Handler
				DCD	 IRQ5_Handler
				DCD	 IRQ6_Handler
				DCD	 IRQ7_Handler
				DCD	 IRQ8_Handler
				DCD	 IRQ9_Handler
				DCD	 IRQ10_Handler
				DCD	 IRQ11_Handler
				DCD	 IRQ12_Handler
				DCD	 IRQ13_Handler
				DCD	 IRQ14_Handler
				DCD	 IRQ15_Handler
				DCD	 IRQ16_Handler
				DCD	 IRQ17_Handler
				DCD	 IRQ18_Handler
				DCD	 IRQ19_Handler
				DCD	 IRQ20_Handler
				DCD	 IRQ21_Handler
				DCD	 IRQ22_Handler
				DCD	 IRQ23_Handler
				DCD	 IRQ24_Handler
				DCD	 IRQ25_Handler
				DCD	 IRQ26_Handler
				DCD	 IRQ27_Handler
				DCD	 IRQ28_Handler
				DCD	 IRQ29_Handler
				DCD	 IRQ30_Handler
				DCD	 IRQ31_Handler

__Vectors_End

__Vectors_Size 	EQU 	__Vectors_End - __Vectors



				AREA	|.text|, CODE, READONLY
				
Reset_Handler   PROC
				EXPORT  Reset_Handler			[WEAK]
				IMPORT  __main
				
				LDR	 R0, =__main
				BX	 R0
				ENDP

				
; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler	 PROC
				EXPORT  NMI_Handler				[WEAK]
				B	   .
				ENDP

HardFault_Handler	PROC
				EXPORT  HardFault_Handler		[WEAK]
				B	   .
				ENDP

SVC_Handler	 PROC
				EXPORT  SVC_Handler			    [WEAK]
				B	   .
				ENDP

PendSV_Handler  PROC
				EXPORT  PendSV_Handler			[WEAK]
				B	   .
				ENDP

SysTick_Handler PROC
				EXPORT  SysTick_Handler		   	[WEAK]
				B	   .
				ENDP

IRQ0_Handler PROC
				EXPORT  IRQ0_Handler		   	[WEAK]
				B	   .
				ENDP

IRQ1_Handler PROC
			  	EXPORT IRQ1_Handler		  		[WEAK]
				B	   .
				ENDP

IRQ2_Handler PROC
			  	EXPORT IRQ2_Handler		  		[WEAK]
				B	   .
				ENDP

IRQ3_Handler PROC
			  	EXPORT IRQ3_Handler		  		[WEAK]
				B	   .
				ENDP

IRQ4_Handler PROC
			  	EXPORT IRQ4_Handler		  		[WEAK]
				B	   .
				ENDP

IRQ5_Handler PROC
			  	EXPORT IRQ5_Handler		  		[WEAK]
				B	   .
				ENDP

IRQ6_Handler PROC
			  	EXPORT IRQ6_Handler		  		[WEAK]
				B	   .
				ENDP

IRQ7_Handler PROC
			  	EXPORT IRQ7_Handler		  		[WEAK]
				B	   .
				ENDP

IRQ8_Handler PROC
			  	EXPORT IRQ8_Handler		  		[WEAK]
				B	   .
				ENDP

IRQ9_Handler PROC
			  	EXPORT IRQ9_Handler		  		[WEAK]
				B	   .
				ENDP

IRQ10_Handler PROC
			  	EXPORT IRQ10_Handler		  	[WEAK]
				B	   .
				ENDP

IRQ11_Handler PROC
			  	EXPORT IRQ11_Handler		  	[WEAK]
				B	   .
				ENDP

IRQ12_Handler PROC
			  	EXPORT IRQ12_Handler		  	[WEAK]
				B	   .
				ENDP

IRQ13_Handler PROC
			  	EXPORT IRQ13_Handler		  	[WEAK]
				B	   .
				ENDP

IRQ14_Handler PROC
			  	EXPORT IRQ14_Handler		  	[WEAK]
				B	   .
				ENDP

IRQ15_Handler PROC
			  	EXPORT IRQ15_Handler		  	[WEAK]
				B	   .
				ENDP

IRQ16_Handler PROC
			  	EXPORT IRQ16_Handler		  	[WEAK]
				B	   .
				ENDP

IRQ17_Handler PROC
				EXPORT  IRQ17_Handler		   	[WEAK]
				B	   .
				ENDP

IRQ18_Handler PROC
				EXPORT  IRQ18_Handler		   	[WEAK]
				B	   .
				ENDP

IRQ19_Handler PROC
				EXPORT  IRQ19_Handler		   	[WEAK]
				B	   .
				ENDP

IRQ20_Handler PROC
				EXPORT  IRQ20_Handler		   	[WEAK]
				B	   .
				ENDP

IRQ21_Handler PROC
				EXPORT  IRQ21_Handler		   	[WEAK]
				B	   .
				ENDP

IRQ22_Handler PROC
				EXPORT  IRQ22_Handler		   	[WEAK]
				B	   .
				ENDP

IRQ23_Handler PROC
				EXPORT  IRQ23_Handler		   	[WEAK]
				B	   .
				ENDP

IRQ24_Handler PROC
				EXPORT  IRQ24_Handler		   	[WEAK]
				B	   .
				ENDP

IRQ25_Handler PROC
				EXPORT  IRQ25_Handler		   	[WEAK]
				B	   .
				ENDP

IRQ26_Handler PROC
				EXPORT  IRQ26_Handler		   	[WEAK]
				B	   .
				ENDP

IRQ27_Handler PROC
				EXPORT  IRQ27_Handler		   	[WEAK]
				B	   .
				ENDP
				ALIGN

IRQ28_Handler PROC
				EXPORT  IRQ28_Handler		   	[WEAK]
				B	   .
				ENDP
				ALIGN

IRQ29_Handler PROC
				EXPORT  IRQ29_Handler		   	[WEAK]
				B	   .
				ENDP
IRQ30_Handler PROC
				EXPORT  IRQ30_Handler		   	[WEAK]
				B	   .
				ENDP

IRQ31_Handler PROC
				EXPORT  IRQ31_Handler		   	[WEAK]
				B	   .
				ENDP
				ALIGN



; User Initial Stack & Heap

				IF	  :DEF:__MICROLIB

				EXPORT  __initial_sp
				EXPORT  __heap_base
				EXPORT  __heap_limit

				ELSE

				IMPORT  __use_two_region_memory
				EXPORT  __user_initial_stackheap
__user_initial_stackheap

				LDR	 R0, =  Heap_Mem
				LDR	 R1, =(Stack_Mem + Stack_Size)
				LDR	 R2, = (Heap_Mem +  Heap_Size)
				LDR	 R3, = Stack_Mem
				BX	 LR

				ALIGN

				ENDIF


				END
