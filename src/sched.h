#ifndef SCHED_H
#define SCHED_H

extern int* saved_registers;

typedef struct pcb_s
{
    int general_registers[13];
    int* next_instruction;
    int LR_user;
    int* SP_user;
    int CPSR_user;
} pcb_s;
typedef int (func_t) (void);

pcb_s* create_process (func_t entry);
void sched_init();
void sys_yieldto(pcb_s* dest);
void do_sys_yieldto();

#endif
