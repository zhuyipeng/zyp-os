#ifndef _CONSOLE_H
#define _CONSOLE_H
#include <stdint.h>
#include <hal.h>
void console_initialize(void);

void console_write(unsigned char*p,int32_t size);
void testPutValue(int value,int base);

#endif
