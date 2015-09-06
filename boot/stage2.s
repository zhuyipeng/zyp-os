.code16
.data
.bss
.text
.globl start
start:
mov $0x11,%ax
.org 511
.word 0x00
#.fill 509,1,0

