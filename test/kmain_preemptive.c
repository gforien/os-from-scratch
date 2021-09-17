#include "syscall.h"
#include "sched.h"

struct pcb_s * p1, * p2, * p3;

void user_process()
{
    int v = 0;
    
    for(;;)
    {
        v++;
    }
}

void kmain( void )
{
    sched_init();

    p1 = create_process((func_t*)&user_process);
    p2 = create_process((func_t*)&user_process);
    p3 = create_process((func_t*)&user_process);

    __asm("cps 0x10"); // switch CPU to USER mode
    // ******************************************

    sys_yield();

}
