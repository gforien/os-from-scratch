#ifndef SCHED_H
#define SCHED_H

extern int* saved_registers;

typedef struct pcb_s
{
    int general_registers[13];
    int* next_instruction;
} pcb_s;
typedef int (func_t) (void);

void sys_yieldto(pcb_s* dest);
void do_sys_yieldto();
pcb_s* create_process (func_t entry);

#endif
