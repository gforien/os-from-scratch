#include "sched.h"

int* saved_registers;

// Syscall number 5
void sys_yieldto(pcb_s* dest)
{
    __asm("mov r0, #5");
    __asm("mov r1, %0" : : "r"(dest) : "r0");
    __asm("swi #0");
}
