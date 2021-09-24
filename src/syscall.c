#include "util.h"
#include "syscall.h"
#include "hw.h"
/**
 * syscall.c
 *
 * "A system call is a primitive function provided by the kernel, able to
 * execute a piece of code requiring privileges inaccessible to user programs."
 *
 * SUMMARY
 *
 * (1) There are several CPU execution modes:
 *  - user (user space)
 *  - system, supervisor, abort, undefined, interrupt, fast interrupt (kernel space)
 *
 * (2) The current mode is stored in the register CPSR (Current Program Status Register).
 *
 * (3) Except system mode, all privileged modes have their own SP and LR.
 * When we switch to a certain mode, $sp and $lr are deactivated (ie $sp or $lr = 0x0)
 * and one must now reference $r13_svc/$r14_svc, or $r13_abt/$r14_abt.
 *
 * (4) If we go to userland with 'cps #16' we cannot come back to a privileged mode
 * (because it is not a privileged mode).
 *
 * (5) Therefore, a system call is a primitive function accessible in user
 * space that redirects to ASM instructions accessible only in kernel space
 * (= in a privileged execution mode).
 *
 * (6) Here is the implementation of syscalls:
 *  - functions sys_reboot(), sys_nop(), ... are called from userland
 *  - they set r0 to 1, 2, 3, ... then call the instruction SWI
 *  - SWI goes to instruction 'asm_swi_handler'
 *  - <init.s> makes 'asm_swi_handler' branch into 'C_swi_handler'
 *  - C_swi_handler() gets value of r0 and calls the corresponding function
 *  - functions do_sys_reboot(), do_sys_nop() call the actual ASM instruction
 *    that must be run in privileged mode.
 *
 */

int* saved_registers;


// Syscall number 4
uint64_t sys_gettime()
{
    __asm("mov r0, #4");
    __asm("swi #0");


    uint32_t lsw;
    uint64_t msw;
    __asm("mov %0, r0" : "=r"(lsw) : : "r1");
    __asm("mov %0, r1" : "=r"(msw) : : "r0");

    uint64_t date = lsw;
    uint64_t date_2 = (msw << 32);
    date = date + date_2;
    return date;
}
void do_sys_gettime()
{
    uint64_t date = get_date_ms();
    uint32_t lsw = (uint32_t) date;
    uint32_t msw = (uint32_t) (date >> 32);
    *(saved_registers) = lsw;
    *(saved_registers+1) = msw;
    //__asm("mov r0, %0" : : "r"(lsw));
    //__asm("mov r1, %0" : : "r"(msw) : "r0");
}


// Syscall number 3
void sys_settime(uint64_t date_ms)
{
    uint32_t lsw = (uint32_t) date_ms;
    uint32_t msw = (uint32_t) (date_ms >> 32);

    __asm("mov r0, #3");
    __asm("mov r1, %0" : : "r"(lsw) : "r0");
    __asm("mov r2, %0" : : "r"(msw) : "r0","r1","r2");
    __asm("swi #0");
}
void do_sys_settime()
{
    // useless computations
    int x1 = 12;
    int x2 = 13;
    x2 += x1;

    // onto the stack:
    // - sp+1 = r1 = lsw
    // - sp+2 = r2 = msw
    // this is our ABI : Application Binary Interface
    uint32_t lsw = *(saved_registers + 1);
    uint64_t msw = *(saved_registers + 2);
    uint64_t date_ms = lsw;
    uint64_t date_ms2 = (msw << 32);
    date_ms = date_ms + date_ms2;

    /* My solution that didn't work
    uint64_t date_ms = *(saved_registers+2);
    date_ms = date_ms << 32;
    uint64_t date_ms2 = *(saved_registers+1);
    date_ms += date_ms2;*/
    set_date_ms(date_ms);
}

// Syscall number 2
void sys_nop()
{
    __asm("mov r0, #2");
    __asm("swi #0");
}
void do_sys_nop()
{
    __asm("nop");
}


// Syscall number 1
void sys_reboot()
{
    __asm("mov r0, #1");
    __asm("swi #0");
    // the instruction SWI require an immediate operand, but we will not use it
    // in the project. In practice, you will always use the syntax SWI #0
}
void do_sys_reboot()
{
    __asm("b _start");
}


// Handler
void __attribute__((naked)) C_swi_handler()
{
    // save user registers
    __asm("stmfd sp!, {r0-r12, lr}");

    // set global variable pointing to the registers
    __asm("mov %0, sp" : "=r"(saved_registers));

    // get the syscall number from r0
    int X;
    __asm("mov %0, r0" : "=r"(X));

    switch(X) {
        case 1:
            do_sys_reboot();
            break;
        case 2:
            do_sys_nop();
            break;
        case 3:
            do_sys_settime();
            break;
        case 4:
            do_sys_gettime();
            break;
        default:
            PANIC();
    }

    // load user registers
    __asm("ldmfd sp!, {r0-r12, pc}^");
}
