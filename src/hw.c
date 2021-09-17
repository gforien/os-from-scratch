#include <stdint.h>

#include "hw.h"
#include "asm_tools.h"

/***************************
 ******** Utilities ********
 ***************************/
uint64_t
divide(uint64_t x, uint64_t y) {
    int quotient = 0;
    while (x >= y) {
        x  -=  y;
        quotient++;
    }
    return quotient;
}

// Memory-Mapped I/O output
static inline void mmio_write(uint32_t reg, uint32_t data)
{
    *(volatile uint32_t*)reg = data;
}
 
// Memory-Mapped I/O input
static inline uint32_t mmio_read(uint32_t reg)
{
    return *(volatile uint32_t*)reg;
}

/* ***************************
 * ********** Timer **********
 * ***************************/
uint64_t
get_date_ms()
{
#if RPI
    uint32_t date_lowbits = Get32(CLO);
    uint64_t date_highbits = (uint64_t) Get32(CHI);
    uint64_t date = divide(((date_highbits << 32) | date_lowbits), SYS_TIMER_CLOCK_div_1000);
#else
    uint64_t date = ((uint64_t) 0x43 << 32) | 0x42;
#endif

    return date;
}

void
set_date_ms(uint64_t date_ms)
{
  uint64_t date = date_ms * SYS_TIMER_CLOCK_div_1000;
  uint32_t date_lowbits = date & (0xFFFFFFFF);
  uint32_t date_highbits = date >> 32;
  Set32(CLO, date_lowbits);
  Set32(CHI, date_highbits);
}

/***********  Generic Timer   ****************/ 
#define CORE0_TIMER_IRQCNTL 0x40000040
#define CORE0_IRQ_SOURCE 0x40000060

static uint32_t cntfrq = 0;

void routing_core0cntv_to_core0irq(void)
{
    mmio_write(CORE0_TIMER_IRQCNTL, 0x08);
}

uint32_t read_core0timer_pending(void)
{
    uint32_t tmp;
    tmp = mmio_read(CORE0_IRQ_SOURCE);
    return tmp;
}

void enable_cntv(void)
{
    uint32_t cntv_ctl;
    cntv_ctl = 1;
    __asm__ volatile ("mcr p15, 0, %0, c14, c3, 1" :: "r"(cntv_ctl) ); // write CNTV_CTL
}

void disable_cntv(void)
{
    uint32_t cntv_ctl;
    cntv_ctl = 0;
    __asm__ volatile ("mcr p15, 0, %0, c14, c3, 1" :: "r"(cntv_ctl) ); // write CNTV_CTL
}

void write_cntv_tval(uint32_t val)
{
  __asm__ volatile ("mcr p15, 0, %0, c14, c3, 0" :: "r"(val) );
  return;
}

uint32_t read_cntfrq(void)
{
    uint32_t val;
    __asm__ volatile ("mrc p15, 0, %0, c14, c0, 0" : "=r"(val) );
    return val;
}

void
clear_and_set_timer()
{
    cntfrq = read_cntfrq();
    write_cntv_tval(cntfrq);    // clear cntv interrupt and set next 1 sec timer.

}

void
timer_init()
{
    clear_and_set_timer();
    routing_core0cntv_to_core0irq();
    enable_cntv(); 
}

/* **************************
 * ********** LEDs **********
 * **************************/
static unsigned int led_state = 0U;

void
led_on(void)
{
    unsigned int *gpclr0_ptr = ((unsigned int*)GPCLR0);
    *gpclr0_ptr = *gpclr0_ptr | 0x10000U;
    led_state = 1;
}

void
led_off(void)
{
    unsigned int *gpclr0_ptr = ((unsigned int*)GPSET0);
    *gpclr0_ptr = *gpclr0_ptr | 0x10000U;
    led_state = 0;
}

void
led_switch(void)
{
    if ((led_state & 1U) == 0U)
    {
        led_off();
    }
    else
    {
        led_on();
    }
}

unsigned int
led_status(void)
{
    return led_state;
}

void
led_init(void)
{
    unsigned int *gpfsel1_ptr = ((unsigned int*)GPFSEL1);
    *gpfsel1_ptr = *gpfsel1_ptr | 0x40000U;
    led_off();
}

void
led_blink(void)
{
    int i =0;
    int waiting = 0;
    for (i = 0; i < 10; i++) {
        led_on();
        for (waiting = 0; waiting < 1000; waiting++);
        led_off();
        for (waiting = 0; waiting < 1000; waiting++);                
    }
}

/* ***********************************
 * ********** init_hw() **************
 * ***********************************/
void
hw_init()
{
    /* Init uart */
    uart_init();

    /* Init LED */
    led_init();
}

void
terminate_kernel()
{
    log_str("Exit kernel\n");
}
