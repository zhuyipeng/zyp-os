.global i86_default_handler,timer_interrupt,keyboard_interrupt
i86_default_handler:
    movb $0x20,%al
	outb %al,$0x20
    iret
timer_interrupt:
    call i86_pit_irq
	movb $0x20,%al
	outb %al,$0x20
	iret

keyboard_interrupt:
    xor %ax,%ax
    call keyboard_scan_code
    movb $0x20,%al
	outb %al,$0x20
    push $0
    #ready call tty_interrupt
    addl $4,%esp
    #ready to call tty_interrupt
    iret