IMAGE=image
all:boot/stage1
	dd if=boot/stage1 of=$(IMAGE) bs=512
boot/stage1:boot/stage1.s
	as boot/stage1.s -o boot/stage1.o
	ld boot/stage1.o -e start --oformat binary -Ttext 0x0 -x -x -o boot/stage1

clean:
	(cd boot;rm *.o stage1)
