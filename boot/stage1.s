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

reset:
	movb $0x0,%ah
	movb $0x0,%dl
	int $0x13
	jc reset
	
	mov $0x1000,%ax
	mov %ax,%es
	xor %bx,%bx
	
read:
	mov %ds:sectors, %ax
	cmp %ax,%ds:sectorcount
	je readover
	imul $0x200,%ax
	mov %ax,%bx
readsect:
	movb $0x02,%ah
	movb $0x01,%al
	movb $0x00,%ch
	movb $0x02,%cl
	movb $0x0,%dh
	movb $0x0,%dl
	int $0x13
	jc readsect
	mov %ds:sectors, %ax
	add $1,%ax
	mov %ax,%ds:sectors
	jmp read
readover:
	jmp $0x1000,$0x0
sectors:
	.word 3
sectorcount:
	.word 6
track:
	.word 0
head:
	.word 0
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
