#ifndef _GDT_H_INCLUDED
#define _GDT_H_INCLUDED

#include <stdint.h>

#define MAX_DESCRIPTORS					3

/***	 gdt descriptor access bit flags.	***/
#define I86_GDT_DESC_ACCESS			0x0001	

#define I86_GDT_DESC_READWRITE			0x0002

#define I86_GDT_DESC_EXPANSION			0x0004	

#define I86_GDT_DESC_EXEC_CODE			0x0008

#define I86_GDT_DESC_CODEDATA			0x0010			//00010000

#define I86_GDT_DESC_DPL			0x0060			//01100000

//! set "in memory" bit
#define I86_GDT_DESC_MEMORY			0x0080			//10000000


/**	gdt descriptor grandularity bit flags	***/
#define I86_GDT_GRAND_LIMITHI_MASK		0x0f			//00001111

//! set os defined bit
#define I86_GDT_GRAND_OS			0x10			//00010000

//! set if 32bit. default: 16 bit
#define I86_GDT_GRAND_32BIT			0x40			//01000000

//! 4k grandularity. default: none
#define I86_GDT_GRAND_4K			0x80			//10000000

struct gdt_descriptor {
	uint16_t		limit;
	uint16_t		baseLo;
	uint8_t			baseMid;
	uint8_t			flags;
	uint8_t			grand;
	uint8_t			baseHi;
}

extern void gdt_set_descriptor(uint32_t i, uint64_t base, uint64_t limit, uint8_t access, uint8_t grand);

extern gdt_descriptor* i86_gdt_get_descriptor (int i);

extern	int i86_gdt_initialize ();

#endif
