#ifndef SCHED_H
#define SCHED_H

extern int* saved_registers;

struct pcb_s
{
    int general_registers[13];
    int* next_instruction;
};
typedef struct pcb_s pcb_s;

void sys_yieldto(pcb_s* dest);
void do_sys_yieldto();
#endif
