OBJS=string.o 
CFLAGS=-nostdlib -fno-builtin -nostdinc -Wall -I../include
CC = gcc
%.o:%.c
	$(CC) -c -o $@ $< $(CFLAGS)
lib.a:$(OBJS)
	ar rcs lib.a $(OBJS)
	sync
string.o:string.c

clean:
	rm -f *.o *.a
