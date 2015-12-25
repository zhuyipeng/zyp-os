#include "idt.h"
#include "pit.h"
#include "pic.h"
#include <hal.h>
#include "console.h"

#define		I86_PIT_REG_COUNTER0		0x40
#define		I86_PIT_REG_COUNTER1		0x41
#define		I86_PIT_REG_COUNTER2		0x42
#define		I86_PIT_REG_COMMAND			0x43

uint32_t			_pit_ticks= 0;

static int _pit_bIsInit = 0;
extern void timer_interrupt();
void i86_pit_irq ();
static int _pit_bIsInit_test = 0;
void i86_pit_irq () {
	//DebugClrScr (0x18);
        //DebugGotoXY (0,0);

        //DebugSetColor (0x1e);

        //DebugPuts ("*** [i86 Hal] i86_default_handler: Unhandled Exception0x20");
	/*
	__asm__("addl $12,%esp\n"
		"pusha"
		);
	
	
	interruptdone(0);
	__asm__("popa\n"
		"iret"
		);
	*/
	_pit_ticks++;
    if(_pit_ticks%10 == 0){
        /*
        if(_pit_bIsInit_test%2 == 0){
            //char *p = "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";;    
            char *p = "123456\t123\r\n";
            console_write(p,strlen(p)); 
        }else if(_pit_bIsInit_test%3 == 0){
            //char *p = "hello67890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";;        
            char *p = "hello\t123\r\n";
            console_write(p,strlen(p)); 
        }else if(_pit_bIsInit_test%7 == 0){
            //char *p = "test167890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";;        
            char*p = "test---\t123\r\n";
            console_write(p,strlen(p)); 
        }
        
        
        _pit_bIsInit_test++;*/
    }
}

uint32_t i86_pit_set_tick_count (uint32_t i) {

	uint32_t ret = _pit_ticks;
	_pit_ticks = i;
	return ret;
}

uint32_t i86_pit_get_tick_count () {
	return _pit_ticks;
}

void i86_pit_send_command (uint8_t cmd) {

	outportb (I86_PIT_REG_COMMAND, cmd);
}

void i86_pit_send_data (uint16_t data, uint8_t counter) {

	uint8_t	port= (counter==I86_PIT_OCW_COUNTER_0) ? I86_PIT_REG_COUNTER0 :
		((counter==I86_PIT_OCW_COUNTER_1) ? I86_PIT_REG_COUNTER1 : I86_PIT_REG_COUNTER2);

	outportb (port, (uint8_t)data);
}

uint8_t i86_pit_read_data (uint16_t counter) {

	uint8_t	port= (counter==I86_PIT_OCW_COUNTER_0) ? I86_PIT_REG_COUNTER0 :
		((counter==I86_PIT_OCW_COUNTER_1) ? I86_PIT_REG_COUNTER1 : I86_PIT_REG_COUNTER2);

	return inportb (port);
}

void i86_pit_start_counter (uint32_t freq, uint8_t counter, uint8_t mode) {

	if (freq==0)
		return;

	uint16_t divisor = (uint16_t)(1193181 / (uint16_t)freq);

	uint8_t ocw=0;
	ocw = (ocw & ~I86_PIT_OCW_MASK_MODE) | mode;
	ocw = (ocw & ~I86_PIT_OCW_MASK_RL) | I86_PIT_OCW_RL_DATA;
	ocw = (ocw & ~I86_PIT_OCW_MASK_COUNTER) | counter;
	i86_pit_send_command (ocw);

	i86_pit_send_data (divisor & 0xff, 0);
	i86_pit_send_data ((divisor >> 8) & 0xff, 0);

	_pit_ticks=0;

}

void  i86_pit_initialize () {
	int i = 0;
	setvect (0x20, timer_interrupt);
	_pit_bIsInit = 1;

	
}

int i86_pit_is_initialized () {

	return _pit_bIsInit;
}
