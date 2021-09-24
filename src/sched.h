#ifndef SCHED_H
#define SCHED_H

struct pcb_s
{
    int general_registers[13];
    int* next_instruction;
};
typedef struct pcb_s pcb_s;

#endif
