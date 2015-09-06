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
#read setup to 0x9000,size 512 bit

	mov $0x9000,%ax
	mov %ax,%es
	xor %bx,%bx
	mov $1,%ax
	mov %ax,%ds:sectorcount
	mov $0,%ax
	mov %ax,%ds:sectors
	mov $1,%ax
	mov %ax,%ds:sectorstart
	call read
#read system to 0x1000,size 5*512 bit
	mov $0x1000,%ax
	mov %ax,%es
	xor %bx,%bx
	mov $5,%ax
	mov %ax,%ds:sectorcount
	mov $0,%ax
	mov %ax,%ds:sectors
	mov $2,%ax
	mov %ax,%ds:sectorstart
	call read
	jmp $0x9000,$0x0
read:
	mov %ds:sectors, %ax
	cmp %ax,%ds:sectorcount
	je readover
	mov %ax,%cx
	imul $0x200,%cx
	mov %cx,%bx
	mov %ds:sectorstart,%ax
	movb $18,%cl
	div %cl
	addb $1,%ah
	movb %ah,%ds:sector
	xorb %ah,%ah
	movb $2,%cl
	div %cl
	movb %ah,%ds:head
	movb %al,%ds:track
readsect:
	movb $0x02,%ah
	movb $0x01,%al
	movb %ds:track,%ch
	movb %ds:sector,%cl
	movb %ds:head,%dh
	movb $0x0,%dl
	int $0x13
	jc readsect
	mov %ds:sectors, %ax
	add $1,%ax
	mov %ax,%ds:sectors
	mov %ds:sectorstart,%ax
	add $1,%ax
	mov %ax,%ds:sectorstart
	jmp read
readover:
	ret

sectors:
	.word 0
sectorcount:
	.word 6
sectorstart:
	.word 1
sector:
	.word 0
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
