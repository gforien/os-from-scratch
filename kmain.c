#include "util.h"
#include "syscall.h"
#include "sched.h"

struct pcb_s *p1, *p2;

void user_process_1()
{
    int v1 = 5;
    while(1)
    {
        v1++;
        sys_yieldto(p2);
    }
}

void user_process_2()
{
    int v2 = -12;
    while(1)
    {
        v2-=2;
        sys_yieldto(p1);
    }
}

void kmain( void )
{
    sched_init();

    func_t* func_process_1 = (func_t*) &user_process_1;
    func_t* func_process_2 = (func_t*) &user_process_2;

    // initialize p1 and p2
    p1 = create_process(func_process_1);
    p2 = create_process(func_process_2);

    // switch CPU to USER mode
    __asm("cps 0x10");
    //**************************
    sys_yieldto(p1);

    // this is now unreachable
    PANIC();
}
