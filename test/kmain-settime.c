#include "stdint.h"
#include "syscall.h"

void kmain( void )
{
    __asm("cps 0x10"); // switch CPU to USER mode
    // **********************************************************************

    sys_settime(0x12345678CACACACA);

}
