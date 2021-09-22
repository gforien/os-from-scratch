#include "src/syscall.h"

void dummy()
{
    return;
}

int div(int dividend, int divisor)
{
    int result = 0;
    int remainder = dividend;
    while (remainder >= divisor) {
        result++;
        remainder -= divisor;
    }
    return result;
}

int compute_volume(int rad)
{
    int rad3 = rad * rad * rad;
    return div(4*355*rad3, 3*113);
}

int __attribute__((naked)) compute_volume2(int rad)
{
    int rad3 = rad * rad * rad;
    return div(4*355*rad3, 3*113);
}



int kmain( void )
{
    /* Chapter 4 : System calls */
    // SYSTEM MODE
    __asm("cps 0x10");

    //******************************/
    // USER MODE
    sys_nop();

    sys_reboot();

    /* Chapter 3 : Execution modes */
    __asm("cps #16");
    __asm("cps #19");

    /* Chapter 2 */
    __asm("bl dummy");
    int var0;
    int var1;
    __asm("mov %0, r0": "=r"(var0): :"r0", "r1");
    __asm("mov %0, r0": "=r"(var1): :"r0", "r1");

    int radius = 5;
    __asm("mov r2, %0": :"r"(radius));
    __asm("mov %0, r3": "=r"(radius));

    int volume;
    dummy();
    volume = compute_volume(radius);

    return volume;
}
