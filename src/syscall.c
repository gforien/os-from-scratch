#include "util.h"
#include "syscall.h"
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


// Syscall number 2
void sys_nop()
{
    __asm("mov r0, #2");
    __asm("swi #0" : : : "r0");
}
void do_sys_nop()
{
    __asm("nop");
}


// Syscall number 1
void sys_reboot()
{
    __asm("mov r0, #1");
    __asm("swi #0" : : : "r0");
    // the instruction SWI require an immediate operand, but we will not use it
    // in the project. In practice, you will always use the syntax SWI #0
}
void do_sys_reboot()
{
    __asm("b _start");
}


// Handler
void C_swi_handler()
{
    // save user registers
    __asm("stmfd sp!, {r1-r12, lr}");

    // get the syscall number from r0
    int X;
    __asm("mov %0, r0" : "=r"(X));

    if (X == 1) {
        do_sys_reboot();
    }
    else if (X == 2) {
        do_sys_nop();
    }
    else {
        PANIC();
    }

    // load user registers
    __asm("ldmfd sp!, {r1-r12, pc}^");
}
