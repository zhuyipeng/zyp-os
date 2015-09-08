.code32
.text
.globl _start
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
	call main
