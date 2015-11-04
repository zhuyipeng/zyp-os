#include <hal.h>

#include "cpu.h"

#include "idt.h"
#include "pic.h"
#include "pit.h"
int hal_initialize () {

	i86_cpu_initialize ();
	i86_pic_initialize (0x20,0x28);
	i86_pit_initialize ();
	i86_pit_start_counter (100,I86_PIT_OCW_COUNTER_0, I86_PIT_OCW_MODE_SQUAREWAVEGEN);
	enable();
	return 0;

}

int hal_shutdown () {

	i86_cpu_shutdown ();

	return 0;

}



void geninterrupt (int n) {

}

inline void interruptdone (unsigned int intno) {

}

void sound (unsigned frequency) {

	//! sets frequency for speaker. frequency of 0 disables speaker
	outportb (0x61, 3 | (unsigned char)(frequency<<2));
}

unsigned char inportb (unsigned short portid) {
	unsigned char _v;
	__asm__ volatile ("inb %%dx,%%al":"=a" (_v):"d" (portid));
	return _v;		
}


void outportb (unsigned short portid, unsigned char value) {
	__asm__ ("outb %%al,%%dx"::"a" (value),"d" (portid));
}

void enable () {
	__asm__("sti");
}

void disable () {
	__asm__("cli");
}

void setvect (int intno, void (far *vect) ( ) ) {
	i86_install_ir (intno, I86_IDT_DESC_PRESENT | I86_IDT_DESC_BIT32,
		0x8, vect);		

}

const char* get_cpu_vender () {
	
}
int get_tick_count () {

	return i86_pit_get_tick_count();
}
