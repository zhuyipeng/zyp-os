.code32
.text
.globl _start,timer_interrupt
_start:
	cli
	mov $0x10,%ax
        mov %ax,%ds
        mov %ax,%es
        mov %ax,%fs
        mov %ax,%gs
	mov %ax,%ss
	mov $0x90000,%esp
	mov %esp,%ebp
	push %ebp
	push $0xffffffff
	call main


timer_interrupt:
	call i86_pit_irq
	movb $0x20,%al
	outb %al,$0x20
	iret	
