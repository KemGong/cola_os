#include <stdio.h>
#include "cola_os.h"
#include "bsp.h"


#define LIST_HEAD_INIT(name) { 0 }

#define TASK_LIST_HEAD(name) \
    struct task_s name = LIST_HEAD_INIT(name)

#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != NULL; pos = pos->next)

#define list_for_null(pos, head) \
    for (pos = head; pos->next != NULL; pos = pos->next)

#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next; pos != NULL; \
          n = pos->next,pos = n)

#define list_for_each_del(pos, n, head) \
    for (n = (head),pos = (head)->next; pos != NULL; \
          n = pos,pos = pos->next)



#define time_after_eq(a,b)  (((int)((a) - (b)) >= 0))

enum
{
    TASK_TASK   = 0x00,
    TASK_TIMER  = 0x01,
};
cbFunc  sleep_handle = NULL;

static TASK_LIST_HEAD(task_list);

volatile unsigned int jiffies = 0;
/*
    查找任务是否存在
*/
static bool cola_task_is_exists( task_t *task )
{
    task_t* cur ;
    list_for_each(cur,&task_list)
    {
        if( cur->timerNum == task->timerNum )
        {
            return true;
        }
    }
    return false;
}

/*
    创建任务
*/
int cola_task_create(task_t *task,cbFunc func,void *arg)
{
    task_t *cur  ;
    OS_CPU_SR cpu_sr;
    enter_critical();
    if((NULL == task )||(cola_task_is_exists(task)))
    {
        exit_critical();
        return false;
    }
    task->taskFlag  = TASK_TASK;
    task->start     = true;
    task->run       = true;
    task->func      = func;
    task->event     = 0;
    task->usr       = arg;
    task->isBusy    = TASK_IDLE;
    list_for_null(cur,&task_list)
    {

    }
    task->timerNum = cur->timerNum+1;
    cur->next  = task;
    task->next = NULL;
    exit_critical();
    return true;
}

/*
  删除指定任务
*/
int cola_task_delete(task_t *task)
{
    task_t *cur,*n;
    OS_CPU_SR cpu_sr;
    enter_critical();
    list_for_each_del(cur,n,&task_list)
    {
        if( cur->timerNum == task->timerNum )
        {
            n->next = cur->next;
            exit_critical();
            return true;
        }
    }
    return false;
}
/*
  循环遍历任务链表
*/
static uint8_t idel_cnt = 0;
void cola_task_loop(void)
{
    uint32_t events =0;
    task_t *cur,*n=NULL ;
    OS_CPU_SR cpu_sr;
    bool busy = 0;
    //list_for_each(cur,&task_list)
    list_for_each_safe(cur,n,&task_list)
    {
        //检查定时任务是否超时
        if((TASK_TIMER == cur->taskFlag)&& (cur->start) && (time_after_eq(jiffies, cur->timerTick)))
        {
            enter_critical();
            cur->run = true;
            cur->timerTick = jiffies+cur->period;
            exit_critical();
        }
        if(cur->run)
        {
            if((cur->oneShot)&&(TASK_TIMER == cur->taskFlag))
            {
               cur->start = false;
            }
            if(NULL !=cur->func)
            {
                events = cur->event;
                if(events)
                {
                    enter_critical();
                    cur->event = 0;
                    exit_critical();
                }
                cur->func(cur->usr,events);
            }
            busy|=cur->isBusy;
            if(TASK_TIMER == cur->taskFlag)
            {
                enter_critical();
                cur->run = false;
                exit_critical();
            }

        }
    }
    if(!busy)
    {
        if(++idel_cnt >2 )
        {
            idel_cnt = 0;
            if(sleep_handle)
            {
                idel_cnt = 0;
                sleep_handle(0,busy);
            }

        }
    }
    else
    {
        idel_cnt = 0;
    }
}

/*
  定时器创建
*/
int cola_timer_create(task_t *timer,cbFunc func,void *arg)
{
    task_t *cur ;
    OS_CPU_SR cpu_sr;
    enter_critical();

    if((NULL == timer )||(cola_task_is_exists(timer)))
    {
        exit_critical();
        return false;
    }
    timer->taskFlag  = TASK_TIMER;
    timer->run       = false;
    timer->period    = 0;
    timer->oneShot   = true;
    timer->start     = false;
    timer->timerTick = 0;
    timer->func      = func;
    timer->event     = 0;
    timer->usr       = arg;
    timer->isBusy    = TASK_IDLE;

    list_for_null(cur,&task_list)
    {

    }
    timer->timerNum = cur->timerNum+1;
    cur->next   = timer;
    timer->next = NULL;
    exit_critical();
    return true;

}

/*
  定时器启动
*/
int cola_timer_start(task_t *timer,bool one_shot,uint32_t time_ms)
{
    task_t *cur ;
    OS_CPU_SR cpu_sr;
    enter_critical();
    list_for_each(cur,&task_list)
    {
        if( cur->timerNum == timer->timerNum )
        {
            timer->period    = time_ms;
            timer->oneShot   = one_shot;
            timer->start     = true;
            timer->timerTick = jiffies + time_ms;
            timer->taskFlag  = TASK_TIMER;
            exit_critical();
            return true;
        }
    }
    exit_critical();
    return false;
}
/*
  定时器停止
*/
int cola_timer_stop(task_t *timer)
{
    task_t *cur;
    OS_CPU_SR cpu_sr;
    enter_critical();
    list_for_each(cur,&task_list)
    {
        if( cur->timerNum == timer->timerNum )
        {
            timer->start     = false;
            exit_critical();
            return true;
        }
    }
    exit_critical();
    return false;
}
/*
  删除指定定时器
*/
int cola_timer_delete(task_t *timer)
{
    return cola_task_delete(timer);
}
void cola_timer_ticker(void)
{
    jiffies++;
}
/*
  设置事件
*/
int cola_set_event(task_t *task,uint32_t sig_id)
{
    task_t *cur;
    OS_CPU_SR cpu_sr;
    enter_critical();
    list_for_each(cur,&task_list)
    {
        if( cur->timerNum == task->timerNum )
        {
            cur->event |= sig_id;
            exit_critical();
            return true;
        }
    }
    exit_critical();
    return false;
}
/*
  清除事件
*/
int  cola_clear_event(task_t *task,uint32_t sig_id)
{
    task_t *cur;
    OS_CPU_SR cpu_sr;
    enter_critical();
    list_for_each(cur,&task_list)
    {
        if( cur->timerNum == task->timerNum )
        {
            cur->event &= ~(sig_id);
            exit_critical();
            return true;
        }
    }
    exit_critical();
    return false;
}
void cola_delay_ms(uint32_t ms)
{
    unsigned int start = jiffies;
    while((jiffies-start) <= ms);
}

int cola_set_sleep_handel(cbFunc func)
{
    sleep_handle = func;
    return 0;
}


