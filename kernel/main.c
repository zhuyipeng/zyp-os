#include "testDisplay.h"
#include <hal.h>
void main(void){
	int i=0x12;
/*
	DebugClrScr (0x18);
	DebugGotoXY (0,0);
	DebugSetColor (0x70);
	DebugPrintf (" Microcomputer Operating System (MOS) Preparing to load...                       ");
	DebugGotoXY (0,1);
	DebugSetColor (0x19);
	DebugPrintf (" MOS Starting Up...\n");

	DebugSetColor (0x70);
	DebugGotoXY (0,24);
	DebugPrintf (" Initializing Hardware Abstraction Layer (HAL.lib)...                           ");

	DebugGotoXY (0,2);

*/
	hal_initialize ();
	//DebugSetColor (0x19);
	//geninterrupt (0x15);
	//__asm__("int $0x10");
	//
	//DebugGotoXY (0,14);
	//i = 0;
	//while(1){
	/*	if(i%2 == 0){
			DebugPrintf ("%d\n",i);
		}else
			DebugPrintf ("%d\n",i);
		i++;*/
	//}
    while(1);
}
