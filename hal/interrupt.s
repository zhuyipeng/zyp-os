.global i86_default_handler,timer_interrupt
i86_default_handler:
    movb $0x20,%al
	outb %al,$0x20
    iret
timer_interrupt:
    call i86_pit_irq
	movb $0x20,%al
	outb %al,$0x20
	iret
