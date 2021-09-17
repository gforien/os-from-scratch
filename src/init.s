// To keep this in the first portion of the binary.
.section ".text.boot"

.balign 32
vector:
    ldr pc, reset_handler
    ldr pc, undefined_handler
    ldr pc, swi_handler
    ldr pc, prefetch_handler
    ldr pc, data_handler
    ldr pc, unused_handler
    ldr pc, irq_handler
    ldr pc, fiq_handler
reset_handler:      .word asm_reset_handler
undefined_handler:  .word hang
swi_handler:        .word asm_swi_handler
prefetch_handler:   .word hang
data_handler:       .word asm_data_handler
unused_handler:     .word hang
irq_handler:        .word asm_irq_handler
fiq_handler:        .word hang

// Make _start global.
.globl _start
 
_start:
asm_reset_handler:
    // in QEMU all of 4 ARM CPUs are started simultaniously
    // by default. I don't know if this is the real hw behaviour,
    // but here I jump to halt if CPU ID (stored in MPIDR
    // register, first 2 bits) is not 0
    mrc p15, #0, r1, c0, c0, #5
    and r1, r1, #3
    cmp r1, #0
    bne hang 

    // set vector address.
    ldr r0, =vector
    mcr P15, 0, r0, c12, c0, 0

    // save CPSR.
    mrs r0, cpsr

    // set sp in irq mode.
    bic r1, r0, #0x1F
    orr r1, r1, #0x12
    msr cpsr_c,r1
    ldr sp, =__irq_stack_end__

   ;@ (PSR_ABORT_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    bic r1, r0, #0x1F
    orr r1, r1, #0x17
    msr cpsr_c,r1
    ldr sp, =__irq_stack_end__

    ;@ (PSR_SVC_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    bic r1, r0, #0x1F
    orr r1, r1, #0x13
    msr cpsr_c,r1
    ldr sp, =__svc_stack_end__

    ;@ (PSR_SYSTEM_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    bic r1, r0, #0x1F
    orr r1, r1, #0x1F
    msr cpsr_c,r1
    ldr sp, =__sys_stack_end__
 
    // Call kernel_main
    ldr r3, =kmain
    blx r3

loop_after_kmain:
    b loop_after_kmain

.globl io_halt
io_halt:
    wfi
    bx lr

asm_irq_handler:
    b asm_irq_handler

asm_swi_handler:
    nop
    b asm_swi_handler

hang:
    wfi
    b hang

asm_data_handler:
	b asm_data_handler

