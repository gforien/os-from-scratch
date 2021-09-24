# -*- mode:gdb-script -*-

layout split
focus cmd
winheight cmd 25
target remote:1234

set architecture arm
set history filename ~/.gdb_history
set history save

b *kmain
#b *sched_init
#b *create_process
#b *sys_yieldto
#b *do_sys_yieldto
b *user_process_1
b *user_process_2
#b sys_settime
#b do_sys_settime
#b set_date_ms
#b *C_swi_handler

#b asm_reset_handler
#b C_irq_handler
b kernel_panic

source utils.gdb

#continue
