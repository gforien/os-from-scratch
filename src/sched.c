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

    // 3. return the new allocated+initialized PCB
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
    // 1) save context, firstly r0-12
    for (int i = 0; i < 13; ++i) {
        *(current_process->general_registers+i) = *(saved_registers+i);
    }
    // then save r14 (i.e. lr) but at position 13
    current_process->next_instruction = (int*)*(saved_registers + 13);

    // 2) change process
    current_process = (pcb_s*) (saved_registers+1);

    // 3) restore context
    for (int i = 0; i < 13; ++i) {
        *(saved_registers+i) = *(current_process->general_registers+i);
    }
    *(saved_registers+13) = (int) current_process->next_instruction;
}
