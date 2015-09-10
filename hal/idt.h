#ifndef _IDT_H
#define _IDT_H
#include <stdint.h>

#define I86_MAX_INTERRUPTS		256


#define I86_IDT_DESC_BIT16		0x06	//00000110

#define I86_IDT_DESC_BIT32		0x0E	//00001110

#define I86_IDT_DESC_RING1		0x40	//01000000

#define I86_IDT_DESC_RING2		0x20	//00100000

#define I86_IDT_DESC_RING3		0x60	//01100000

#define I86_IDT_DESC_PRESENT		0x80	//10000000

typedef void (*I86_IRQ_HANDLER)(void);

struct idt_descriptor {
	uint16_t		baseLo;

	uint16_t		sel;

	uint8_t			reserved;

	uint8_t			flags;

	uint16_t		baseHi;

};

//! returns interrupt descriptor
extern idt_descriptor* i86_get_ir (uint32_t i);
//
//! installs interrupt handler. When INT is fired, it will call this callback

extern int i86_install_ir (uint32_t i, uint16_t flags, uint16_t sel, I86_IRQ_HANDLER);
// initialize basic idt
extern int i86_idt_initialize (uint16_t codeSel);

#endif

