#ifndef _HAL_H
#define _HAL_H

#include <stdint.h>

extern	int hal_initialize ();

extern	int hal_shutdown ();

extern	void geninterrupt (int n);


#endif
