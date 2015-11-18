#include "console.h"

static uint16_t video_port_regs = 0x3d4;
static uint16_t video_value_regs = 0x3d5; 
static uint16_t video_model_control_regs = 0x3d8;
static uint32_t video_mem_start = 0x0;
static uint32_t video_mem_end = 0x0;
static uint32_t video_mem_origin = 0x0;
static int32_t x = 0;
static int32_t y = 0;
static char *position = 0;
static int top = 0;
static int bottom = 0;
static uint8_t attr = 0x07;
static uint32_t screen_column_num = 0;
static uint32_t screen_row_num = 0;
static int state = 0;
static void cursor_initalize(void){
	
}

static void gotoxy(int32_t x,int32_t y){
	

}

static void init_video_mem_start(){
	//outportb(video_port_regs,12);
	video_mem_start = 0xb8000;
	video_mem_origin = 0xb8000;
	position = 0xb8000;
}

static void init_video_mem_end(){
	video_mem_start = 0xba000;
}
static void init_screen_size(){
	unsigned char value = inportb(video_model_control_regs);
	//if(value&0x02)
	//	return;
	if(value&0x01){
		screen_column_num = 80;
	}else{
		screen_column_num = 40;
	}
	screen_row_num = 25;
	bottom = 25;
}


void console_initialize(void){
	init_video_mem_start();
	init_video_mem_end();
	init_screen_size();
			
}

static void scrup(void){
	
}

static void cr(void){
	position -= x << 1;
	x = 0;
}

static void lf(void){ //line feed
	if(y + 1 < bottom){
		y++;
		position += 80*2;//(screen_column_num*2);
		return;
	}
	scrup();
}

void console_write(unsigned char*p,int32_t size){
	int i = 0;
	for(i = 0;i < size;i++){
		char c = p[i];
		switch(state){
			case 0:
				if(c == 27) //switch 1
					state = 1;
				else if(c == 10 || c == 11 || c ==12){
					lf();
				}else if(c == 13){
					cr();
				}
				else if(c > 31 && c < 127){
					if(x >= screen_column_num){
						x = 0;
						position -= (screen_column_num*2);
						lf();	
					}
					*position = c;
					*(position+1) = attr;
					position += 2;
					x++;
				}
				break;
			case 1:
				break;
			default:
				break;
		} 
	}		

}
