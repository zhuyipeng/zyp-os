.code32
.text
.globl _start,
_start:
	cli
	mov $0x10,%ax
    mov %ax,%ds
    mov %ax,%es
    mov %ax,%fs
    mov %ax,%gs
	mov %ax,%ss
	mov $0x9FFFF,%esp
	mov %esp,%ebp
	push %ebp
	push $0xffffffff
	call main
		
