.code16
.text
.bss
.data
.text
.globl start
BOOTSEG = 0x7c0

start:
	mov $BOOTSEG,%ax
	mov %ax,%ds
	mov %ax,%es
	mov $msg,%si
	call print
	#mov %ax,%cs
	jmp loader

loader:
1:
	jmp 1

print:
        lodsb
        or %al,%al
        jz printDone
        movb $0xe,%ah
        int $0x10
        jmp print
printDone:
        ret


msg:
	.byte 13,10
        .ascii "Loading system ... compiled by zyp!"
        .byte 13,10,13,10,0

.org 510
.word 0xAA55
