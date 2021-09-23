#include <stdint.h>

#ifndef SYSCALL_H
#define SYSCALL_H

void sys_settime(uint64_t date_ms);
void do_sys_settime();

void sys_nop();
void do_sys_nop();

void sys_reboot();
void do_sys_reboot();

void C_swi_handler();

#endif
