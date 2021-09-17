#include "util.h"
#include "syscall.h"

void sys_reboot()
{
    __asm("mov r0, #1");
    __asm("swi #0" : : : "r0");
}

void C_swi_handler()
{
    int X;
    __asm("mov %0, r0" : "=r"(X));

    ASSERT(X == 1);
    do_sys_reboot();
}

void do_sys_reboot()
{
    __asm("b _start");
}
