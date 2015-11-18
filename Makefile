IMAGE=image
LIBS=lib/lib.a
all:boot/stage1 boot/stage2 system
	dd if=boot/stage1 of=$(IMAGE) bs=512
	dd if=boot/stage2 of=$(IMAGE) seek=512 bs=1 count=512
	dd if=system of=$(IMAGE) seek=1024 bs=1 count=5120
boot/stage1:boot/stage1.s system
#echo -n "SYSSIZE="; ls -l system |grep system|cut -c25-31|tr \n ' '|tr -d [:alpha:]>tmp.s
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
	gcc kernel/main.c -c -o kernel/main.o -Iinclude
kernel/testDisplay.o:kernel/testDisplay.c
	gcc kernel/testDisplay.c -c -o kernel/testDisplay.o -I../include
HALOBJS = hal/hal.o hal/gdt.o hal/idt.o hal/cpu.o hal/pic.o hal/pit.o hal/console.o
HALCFLAGS= -nostdinc -Iinclude 
hal/halall.o:$(HALOBJS)
	ld -r -o hal/halall.o $(HALOBJS)

$(HALOBJS):
	gcc $(HALCFLAGS) -c $*.c -o $*.o
$(LIBS):lib/string.c
	(cd lib;make)

clean:
	rm image system* -f
	(cd hal;rm *.o -f)
	(cd kernel;rm *.o -f)
	(cd boot;rm *.o stage1 stage2 -f)
	(cd lib;make clean)
