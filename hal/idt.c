
#include "idt.h"

#include <string.h>

#include <hal.h>

#ifdef _DEBUG
#include "..\Kernel\DebugDisplay.h"
#endif

struct idtr {
	uint16_t		limit;
	uint32_t		base;	
};

static idt_descriptor	_idt [I86_MAX_INTERRUPTS];

static struct idtr _idtr;

static void idt_install ();

static void i86_default_handler ();

static void idt_install () {
//	_asm lidt [_idtr]
}

static void i86_default_handler () {



#ifdef _DEBUG

	DebugClrScr (0x18);

	DebugGotoXY (0,0);

	DebugSetColor (0x1e);

	DebugPuts ("*** [i86 Hal] i86_default_handler: Unhandled Exception");

#endif

	for(;;);

}

idt_descriptor* i86_get_ir (uint32_t i) {

	if (i>I86_MAX_INTERRUPTS)

		return 0;

	return &_idt[i];

}

int i86_install_ir (uint32_t i, uint16_t flags, uint16_t sel, I86_IRQ_HANDLER irq) {
	
	if (i>I86_MAX_INTERRUPTS)
		return 0;

	if (!irq)
		return 0;

	uint64_t		uiBase = (uint64_t)&(*irq);
	
	_idt[i].baseLo		=	(uint16_t)(uiBase & 0xffff);

	_idt[i].baseHi		=	(uint16_t)((uiBase >> 16) & 0xffff);

	_idt[i].reserved	=	0;

	_idt[i].flags		=	(uint8_t)(flags);

	_idt[i].sel		=	sel;

	return 0;	
}

int i86_idt_initialize (uint16_t codeSel) {

	_idtr.limit = sizeof (idt_descriptor) * I86_MAX_INTERRUPTS -1;

	_idtr.base	= (uint32_t)&_idt[0];
	memset ((void*)&_idt[0], 0, sizeof (idt_descriptor) * I86_MAX_INTERRUPTS-1);
	int i = 0;
	for (i=0; i<I86_MAX_INTERRUPTS; i++)
		i86_install_ir (i, I86_IDT_DESC_PRESENT | I86_IDT_DESC_BIT32,
			codeSel, (I86_IRQ_HANDLER)i86_default_handler);
	idt_install ();
	return 0;

}



