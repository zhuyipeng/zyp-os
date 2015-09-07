IMAGE=image
all:boot/stage1 boot/stage2 system
	dd if=boot/stage1 of=$(IMAGE) bs=512
	dd if=boot/stage2 of=$(IMAGE) seek=512 bs=1 count=512
	dd if=system of=$(IMAGE) seek=1024 bs=1 count=512
boot/stage1:boot/stage1.s
	as boot/stage1.s -o boot/stage1.o
	ld boot/stage1.o -e start --oformat binary -Ttext 0x0 -x -x -o boot/stage1

boot/stage2:boot/stage2.s
	as boot/stage2.s -o boot/stage2.o
	ld boot/stage2.o -e start --oformat binary -Ttext 0x0 -x -x -o boot/stage2

system: boot/head.o
	ld boot/head.o -Tld.lds -x -s -M -o system > system.map

boot/head.o:boot/head.s
	as boot/head.s -o boot/head.o
clean:
	rm image system* -f
	(cd boot;rm *.o stage1 stage2)
