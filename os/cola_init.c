#include "cola_init.h"



void do_init_call(void)
{
//#if defined(__CC_ARM)                         /* ARM Compiler */
    extern initcall_t initcall0init$$Base[];
    extern initcall_t initcall0init$$Limit[];
    extern initcall_t initcall1init$$Base[];
    extern initcall_t initcall1init$$Limit[];
    extern initcall_t initcall2init$$Base[];
    extern initcall_t initcall2init$$Limit[];


    initcall_t *fn;

    for (fn = initcall0init$$Base;
            fn < initcall0init$$Limit;
            fn++)
    {
        if(fn)
            (*fn)();
    }

    for (fn = initcall1init$$Base;
            fn < initcall1init$$Limit;
            fn++)
    {
        if(fn)
            (*fn)();
    }

    for (fn = initcall2init$$Base;
            fn < initcall2init$$Limit;
            fn++)
    {
        if(fn)
            (*fn)();
    }
//#elif defined (__GNUC__)
//    extern initcall_t __initcall_start[];
//    extern initcall_t __initcall_end[];

//    initcall_t *start = __initcall_start;
//    initcall_t *end = __initcall_end;
//    initcall_t *fn;

//    for (fn = start; fn < end; fn++)
//    {
//        printf("initcall fn 0x%x\r\n", fn);
//        (*fn)();
//    }
// #endif // defined
}


void do_app_init_call(void)
{
//#if defined(__CC_ARM_)
    initcall_t *fn;
    extern initcall_t initcall3init$$Base[];
    extern initcall_t initcall3init$$Limit[];
    for (fn = initcall3init$$Base;
    fn < initcall3init$$Limit;
    fn++)
    {
        if(fn)
        (*fn)();
    }
//#endif
}
