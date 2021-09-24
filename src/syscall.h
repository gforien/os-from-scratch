#include <stdint.h>

#ifndef SYSCALL_H
#define SYSCALL_H

// syscall n4
uint64_t sys_gettime();
void do_sys_gettime();

// syscall n3
void sys_settime(uint64_t date_ms);
void do_sys_settime();

// syscall n2
void sys_nop();
void do_sys_nop();

// syscall n1
void sys_reboot();
void do_sys_reboot();

void C_swi_handler();

#endif
