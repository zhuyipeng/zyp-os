#for setup
.code16
.data
.bss
.text
.globl start
start:
	mov $0x9020,%ax
	mov %ax,%ds
	mov $0x03,%ah
	xor %bh,%bh
	int $0x10
	mov %dx,(0)

#get memory size
	mov $0x88,%ah
	int $0x15
	mov %ax,(2)
	
#get video-card data
	
	mov $0x0f,%ah
	int $0x10
	mov %bx,(4)
    mov %ax,(6)
	
#get for EGA/VGA
	mov $0x12,%ah
    mov $0x10,%bl
    int $0x10
    mov %ax,(8)
    mov %bx,(10)
    mov %cx,(12)

#get hd0 data

	mov $0x0000,%ax
    mov %ax,%ds
    lds (4*0x41),%si
    mov $0x9020,%ax
    mov %ax,%es
    mov $0x0080,%di
    mov $0x10,%cx
    rep movsb
#start system move
	cli
	xor %ax,%ax
	cld
do_move:
	mov %ax,%es
	add $0x1000,%ax
	cmp $0x9000,%ax
	jz end_move
	mov %ax,%ds
        sub %di,%di
        sub %si,%si
        mov $0x8000,%cx
        rep movsw
        jmp do_move
end_move:
	mov $0x9000,%ax
	mov %ax,%ds
	lidt (ldt_48)
	lgdt (gdt_48)

	call empty_8042
        mov $0xd1,%al
        out %al,$0x64
        call empty_8042
        mov $0xdf,%al
        out %al,$0x60
        call empty_8042

        mov     $0x11,%al                # initialization sequence
        out     %al,$0x20                # send it to 8259A-1
        .word   0x00eb,0x00eb           # jmp $+2, jmp $+2
        out     %al,$0xA0                # and to 8259A-2
        .word   0x00eb,0x00eb
        mov     $0x20,%al                # start of hardware int's (0x20)
        out     %al,$0x21
        .word   0x00eb,0x00eb
        mov     $0x28,%al                # start of hardware int's 2 (0x28)
	out     %al,$0xA1
        .word   0x00eb,0x00eb
        mov     $0x04,%al                # 8259-1 is master
        out     %al,$0x21
        .word   0x00eb,0x00eb
        mov     $0x02,%al                # 8259-2 is slave
        out     %al,$0xA1
        .word   0x00eb,0x00eb
        mov     $0x01,%al                # 8086 mode for both
        out     %al,$0x21
        .word   0x00eb,0x00eb
        out     %al,$0xA1
        .word   0x00eb,0x00eb
        mov     $0xFF,%al                # mask off all interrupts for now

	out     %al,$0x21
        .word   0x00eb,0x00eb
        out     %al,$0xA1

        mov $0x0001,%ax
        lmsw %ax
	ljmp $0x8,$0

empty_8042:
        .word 0x00eb,0x00eb
        in $0x64,%al
        test $2,%al
        jnz empty_8042
        ret

gdt:
        .word 0,0,0,0
        .word 0x07ff
        .word 0x0000
        .word 0x9a00
        .word 0x00c0
        .word 0x07ff
        .word 0x0000
        .word 0x9200
        .word 0x00c0
ldt_48:
        .word 0
        .word 0,0
gdt_48:
        .word 0x800
        .word gdt,0x9	
.org 510
.word 0x00
#.fill 509,1,0

