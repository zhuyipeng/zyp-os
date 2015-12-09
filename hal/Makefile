HALOBJS = hal.o gdt.o idt.o cpu.o pic.o pit.o console.o interrupt.o
CFLAGS = -nostdlib -fno-builtin -nostdinc -I../include
halall.o:$(HALOBJS)
	ld -r -o halall.o $(HALOBJS)
	sync
%.o:%.c $(DEPS)
	gcc -c -o $@ $< $(CFLAGS)
%.o:%.s
	as -o $@ $<
clean:
	rm *.o *.d tmp_make -rf
dep:
	sed '/\#\#\# Dependencies/q' < Makefile > tmp_make
	(for i in *.c;do \
		gcc -M -I../include $$i;done) >> tmp_make
	(for i in *.s;do \
		echo -n `echo $$i |sed 's,\.s,\.o,'` ":$$i";done) >> tmp_make
	cp tmp_make Makefile
### Dependencies:
console.o: console.c console.h ../include/stdint.h ../include/hal.h \
 ../kernel/testDisplay.h
cpu.o: cpu.c cpu.h ../include/stdint.h gdt.h idt.h
gdt.o: gdt.c gdt.h ../include/stdint.h ../include/string.h \
 ../include/size_t.h ../kernel/testDisplay.h
hal.o: hal.c ../include/hal.h ../include/stdint.h cpu.h idt.h pic.h pit.h \
 console.h
idt.o: idt.c idt.h ../include/stdint.h ../include/string.h \
 ../include/size_t.h ../include/hal.h
pic.o: pic.c ../include/hal.h ../include/stdint.h pic.h
pit.o: pit.c idt.h ../include/stdint.h pit.h pic.h ../include/hal.h \
 console.h
interrupt.o :interrupt.s