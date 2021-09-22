#ifndef SYSCALL_H
#define SYSCALL_H

void sys_nop();
void do_sys_nop();

void sys_reboot();
void do_sys_reboot();

void C_swi_handler();

#endif
