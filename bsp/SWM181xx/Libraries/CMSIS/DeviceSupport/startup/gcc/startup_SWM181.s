/* Sram Switch Configuration
            Code Size       Sram Size
    0       16k             16k             Cache Enable，Code Memory 240K
    1       8k              24k             Cache Enable，Code Memory 240K
    3       24k             8k                                          */
    .equ SRAM_SWITCH, 3


    .syntax unified
    .arch armv6-m

/* Memory Model
   The HEAP starts at the end of the DATA section and grows upward.
   
   The STACK starts at the end of the RAM and grows downward     */
    .section .stack
    .align 3
    .globl    __StackTop
    .globl    __StackLimit
__StackLimit:
    .space    0x800
__StackTop:


    .section .heap
    .align 3
    .globl    __HeapBase
    .globl    __HeapLimit
__HeapBase:
    .space    0x000
__HeapLimit:


    .section .isr_vector
    .align 2
    .globl __isr_vector
__isr_vector:
    .long    __StackTop            
    .long    Reset_Handler         
    .long    NMI_Handler          
    .long    HardFault_Handler     
    .long    0     
    .long    0      
    .long    0   
    .long    0                    
    .long    0x0B11FFAC
    .long    0x6000
    .long    SRAM_SWITCH
    .long    SVC_Handler          
    .long    0     
    .long    0                     
    .long    PendSV_Handler           
    .long    SysTick_Handler         

    /* External interrupts */
    .long     IRQ0_Handler
    .long     IRQ1_Handler
    .long     IRQ2_Handler
    .long     IRQ3_Handler
    .long     IRQ4_Handler
    .long     IRQ5_Handler
    .long     IRQ6_Handler
    .long     IRQ7_Handler
    .long     IRQ8_Handler
    .long     IRQ9_Handler
    .long     IRQ10_Handler
    .long     IRQ11_Handler
    .long     IRQ12_Handler
    .long     IRQ13_Handler
    .long     IRQ14_Handler
    .long     IRQ15_Handler
    .long     IRQ16_Handler
    .long     IRQ17_Handler
    .long     IRQ18_Handler
    .long     IRQ19_Handler
    .long     IRQ20_Handler
    .long     IRQ21_Handler
    .long     IRQ22_Handler
    .long     IRQ23_Handler
    .long     IRQ24_Handler
    .long     IRQ25_Handler
    .long     IRQ26_Handler
    .long     IRQ27_Handler
    .long     IRQ28_Handler
    .long     IRQ29_Handler
    .long     IRQ30_Handler
    .long     IRQ31_Handler


	.section .text.Reset_Handler
    .align 2
    .thumb
    .globl    Reset_Handler
    .type     Reset_Handler, %function
Reset_Handler:
/* Loop to copy data from read only memory to RAM. The ranges
 * of copy from/to are specified by symbols evaluated in linker script.  */
    ldr    r1, =__data_load__
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

    subs   r3, r2
    ble .Lflash_to_ram_done

.Lflash_to_ram_loop:
    subs r3, #4
    ldr r0, [r1, r3]
    str r0, [r2, r3]
    bgt .Lflash_to_ram_loop
.Lflash_to_ram_done:


    ldr    r2, =__bss_start__
    ldr    r3, =__bss_end__

    subs   r3, r2
    ble .Lbss_to_ram_done

    movs    r0, 0
.Lbss_to_ram_loop:
    subs r3, #4
    str r0, [r2, r3]
    bgt .Lbss_to_ram_loop
.Lbss_to_ram_done:

    ldr    r0, =main
    bx     r0
    .pool    


    .text
/* Macro to define default handlers. 
   Default handler will be weak symbol and just dead loops. */
    .macro    def_default_handler    handler_name
    .align 1
    .thumb_func
    .weak    \handler_name
    .type    \handler_name, %function
\handler_name :
    b    .
    .endm

    def_default_handler    NMI_Handler
    def_default_handler    HardFault_Handler
    def_default_handler    SVC_Handler
    def_default_handler    PendSV_Handler
    def_default_handler    SysTick_Handler

    def_default_handler    IRQ0_Handler
    def_default_handler    IRQ1_Handler
    def_default_handler    IRQ2_Handler
    def_default_handler    IRQ3_Handler
    def_default_handler    IRQ4_Handler
    def_default_handler    IRQ5_Handler
    def_default_handler    IRQ6_Handler
    def_default_handler    IRQ7_Handler
    def_default_handler    IRQ8_Handler
    def_default_handler    IRQ9_Handler
    def_default_handler    IRQ10_Handler
    def_default_handler    IRQ11_Handler
    def_default_handler    IRQ12_Handler
    def_default_handler    IRQ13_Handler
    def_default_handler    IRQ14_Handler
    def_default_handler    IRQ15_Handler
    def_default_handler    IRQ16_Handler
    def_default_handler    IRQ17_Handler
    def_default_handler    IRQ18_Handler
    def_default_handler    IRQ19_Handler
    def_default_handler    IRQ20_Handler
    def_default_handler    IRQ21_Handler
    def_default_handler    IRQ22_Handler
    def_default_handler    IRQ23_Handler
    def_default_handler    IRQ24_Handler
    def_default_handler    IRQ25_Handler
    def_default_handler    IRQ26_Handler
    def_default_handler    IRQ27_Handler
    def_default_handler    IRQ28_Handler
    def_default_handler    IRQ29_Handler
    def_default_handler    IRQ30_Handler
    def_default_handler    IRQ31_Handler

    def_default_handler    Default_Handler

    .end
