# -*- mode:gdb-script -*-

layout split
focus cmd
winheight cmd 25
target remote:1234

set architecture arm
set history filename ~/.gdb_history
set history save

b *kmain
b do_sys_reboot
b asm_swi_handler
b C_swi_handler
#b asm_reset_handler
#b C_irq_handler
#b kernel_panic

source utils.gdb

#continue
