#ifndef _HAL_H
#define _HAL_H

#include <stdint.h>

#define far
#define near


extern	int hal_initialize ();

extern	int hal_shutdown ();

extern	void geninterrupt (int n);

extern	void interruptdone (unsigned int intno);

extern	void sound (unsigned frequency);

extern	unsigned char inportb (unsigned short portid);

extern void outportb (unsigned short portid, unsigned char value);

extern void enable();

extern void disable();

extern void setvect (int intno, void (far *vect)() );

//extern void (far *getvect (int intno)) ( );
//

extern const char* get_cpu_vender ();

extern int get_tick_count ();




#endif
