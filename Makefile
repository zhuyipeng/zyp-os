IMAGE=image
LIBS=lib/lib.a
all:boot/stage1 boot/stage2 system
	dd if=boot/stage1 of=$(IMAGE) bs=512
	dd if=boot/stage2 of=$(IMAGE) seek=512 bs=1 count=512
	dd if=system of=$(IMAGE) seek=1024 bs=1 count=2560
boot/stage1:boot/stage1.s
	as boot/stage1.s -o boot/stage1.o
	ld boot/stage1.o -e start --oformat binary -Ttext 0x0 -x -x -o boot/stage1

boot/stage2:boot/stage2.s
	as boot/stage2.s -o boot/stage2.o
	ld boot/stage2.o -e start --oformat binary -Ttext 0x0 -x -x -o boot/stage2

system: boot/head.o kernel/main.o kernel/testDisplay.o hal/halall.o $(LIBS)
	ld boot/head.o kernel/main.o kernel/testDisplay.o hal/halall.o $(LIBS) -Tld.lds -x -s -M -o system > system.map

boot/head.o:boot/head.s
	as boot/head.s -o boot/head.o
kernel/main.o:kernel/main.c
	gcc kernel/main.c -c -o kernel/main.o -I../include
kernel/testDisplay.o:kernel/testDisplay.c
	gcc kernel/testDisplay.c -c -o kernel/testDisplay.o -I../include
HALOBJS = hal/hal.o hal/gdt.o hal/idt.o hal/cpu.o
hal/halall.o:$(HALOBJS)
	ld -r -o hal/halall.o $(HALOBJS)
hal/.c.o:hal/hal.c hal/gdt.c hal/idt.c hal/cpu.c hal/hal.h hal/gdt.h hal/idt.h hal/cpu.h
	gcc -c -o $*.o $*.c -I../include
$(LIBS):lib/string.c
	(cd lib;make)

clean:
	rm image system* -f
	(cd boot;rm *.o stage1 stage2)
	(cd lib;make clean)
	(cd hal;rm *.o)
