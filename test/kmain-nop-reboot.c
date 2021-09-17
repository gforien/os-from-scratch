#include "syscall.h"
#include "asm_tools.h"

void kmain( void )
{
    __asm("cps 0x10"); // switch CPU to USER mode

    sys_nop();

    sys_reboot();

}
