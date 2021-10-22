#include "kheap.h"
#include "sched.h"

pcb_s* current_process;
pcb_s* kmain_process;

pcb_s* create_process (func_t* entry)
{
    // 1. allocate a new PCB
    pcb_s* new_pcb = (pcb_s*) kAlloc(sizeof(pcb_s));

    // 2. initialize it
    new_pcb->next_instruction = (int*) entry;

    // 3. Add a new 10KB execution stack
    int* new_stack = (int*) kAlloc(sizeof(10000));
    // we substract 2500 int (= 10 KB) to point to the top of the stack
    new_pcb->SP_user = new_stack - 2500;

    // 4. return the new allocated+initialized PCB
    return new_pcb;
}

void sched_init()
{
    kheap_init();
    kmain_process = (pcb_s*) kAlloc(sizeof(pcb_s));
    current_process = kmain_process;
    // kFree(saved_registers, sizeof(pcb_s));
}

// Syscall number 5
void sys_yieldto(pcb_s* dest)
{
    __asm("mov r0, #5");
    __asm("mov r1, %0" : : "r"(dest) : "r0");
    __asm("swi #0");
}
void do_sys_yieldto()
{
    // 1) Switch to System mode (31) to save LR_user and SP_user
    __asm("cps #31");
    __asm("mov %0, lr" : "=r"(current_process->LR_user));
    __asm("mov %0, sp" : "=r"(current_process->SP_user));
    // then switch back to SVC mode (19)
    __asm("cps #19");
    // Also save the CPSR (which has been copied to SVC SPSR)
    __asm("mrs %0, spsr" : "=r"(current_process->CPSR_user)); // 1

    // 2) save context, firstly r0-12
    for (int i = 0; i < 13; ++i) {
        *(current_process->general_registers+i) = *(saved_registers+i);
    }
    // then save r14 (i.e. lr) but at position 13
    current_process->next_instruction = (int*)*(saved_registers + 13);


    // 2) change process
    current_process = (pcb_s*) *(saved_registers+1);


    // 3) restore context : same as step 2 but reversed
    for (int i = 0; i < 13; ++i) {
        *(saved_registers+i) = *(current_process->general_registers+i);
    }
    *(saved_registers+13) = (int) current_process->next_instruction;
    __asm("cps #31");
    __asm("mov lr, %0" :  : "r"(current_process->LR_user));
    __asm("mov sp, %0" :  : "r"(current_process->SP_user));
    // Also restore the CPSR
    __asm("msr cpsr, %0" : : "r"(current_process->CPSR_user));
    __asm("cps #19");
}
