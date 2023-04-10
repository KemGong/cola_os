#include "config.h"
#include "bsp.h"
#include "cola_os.h"
#include "app.h"
int main(void)
{
    disable_irq();
    bsp_init();
    enable_irq();
    //bsp_app_init();
    app_init();
    while(1)
    {
        cola_task_loop();
    }
}
