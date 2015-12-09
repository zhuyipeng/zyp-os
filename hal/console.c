#include "console.h"
#include <hal.h>
#define _DEBUG
#ifdef _DEBUG
#include "../kernel/testDisplay.h"
#endif
/*static uint16_t video_port_regs = 0x3d4;
static uint16_t video_value_regs = 0x3d5; */
static uint16_t video_port_regs = 0x3d4;
static uint16_t video_value_regs = 0x3d5; 

static uint16_t video_model_control_regs = 0x3d8;
static uint32_t video_mem_start = 0x0;
static uint32_t video_mem_end = 0x0;
static uint32_t video_mem_origin = 0x0;
static int32_t x = 0;
static int32_t y = 0;
static uint32_t position = 0;
static int top = 0;
static int bottom = 0;
static uint8_t attr = 0x07;
static uint32_t screen_column_num = 0;
static uint32_t screen_row_num = 0;
static int state = 0;
static int scr_gap = 1;
static short erase_char = 0x0720;

static void cursor_initalize(void){
	int a = 0;
}

static void itoa(long value,char *p,int size,int base){
    
    const char* digits ="0123456789ABCDEF";
    char tmp[35];
    int i = 0;
    memset((void*)tmp,0,35);
    
    while(value){
        tmp[i++] = digits[value%base];
        value /= base;
    }
    if(base == 16){
        tmp[i++] = 'X';
        tmp[i++] = '0';
    }
    if(size < i)
        return;
    while(i){
        *p++ = *(tmp+i-1);
        i--;
    }
}

static void testPutChar(char *pos,char *a, int size){
    int i = 0;
    for(i = 0; i < size; i++){
        char *tmp = pos+i*sizeof(short);
        *tmp = a[i];
        *(tmp+1) = 0x07;
    }
    
}

static void testPutValue(char *pos, int value,int base){
    char tmp[35];
    memset(tmp,0,35);
    itoa(value,tmp,35,base);
    testPutChar(pos, tmp, strlen(tmp));
}

static void gotoxy(int32_t tmpX,int32_t tmpY){
	if(x >= screen_column_num || y >= screen_row_num) {
        return;
    }
    x = tmpX;
    y = tmpY;
    position = video_mem_origin+ y * screen_column_num * sizeof(short) + x*sizeof(short);
}

static void init_video_mem(){
    //outportb(video_port_regs,12);
    unsigned char *display_size = 0x9020a;
    video_mem_start = 0xb8000;
    video_mem_origin = 0xb8000;
    int size = 0x0;
    if(*display_size == 0x0){
        size = 64;
    }else if(*display_size == 0x1){
        size = 128;
    }else if(*display_size == 0x2){
        size = 196;
    }else if(*display_size == 0x3){
        size = 256;
    }
    video_mem_end = 0xBC000;
    position = video_mem_start;
}

static void init_screen_size(){
    screen_column_num = 80;
    screen_row_num = 25;
    bottom = 24;
}
static void set_origin(){
    disable();
    outportb(video_port_regs,12);
    outportb(video_value_regs,0xff&((video_mem_origin - video_mem_start) >> 9));
    outportb(video_port_regs,13);
    outportb(video_value_regs,0xff&((video_mem_origin - video_mem_start) >> 1));
    enable();
}

static void set_cursor(){
    disable();
    outportb(video_port_regs,14);
    outportb(video_value_regs,0xff&((position - video_mem_start) >> 9));
    outportb(video_port_regs,15);
    outportb(video_value_regs,0xff&((position - video_mem_start) >> 1));
    enable();
}

static void clear_screen(long start,long end){
    /*
    int i = 0, j = 0;
    int row = count/80;
    for(i = 0; i < row; i++){
        for( j = 0; j < screen_column_num; j++){
            short *p = (short*)(start+(i*screen_column_num + j)*sizeof(short));
            *p = erase_char;
        }
    }*/
    if(end <= start){
        return;
    }
    long size = end - start;
    short *p = (short*)start;
    while(p <= end){
        *p = erase_char;
        p++;
    }
}




static void scrup(void){
    if(video_mem_start + ((position - video_mem_start +2*screen_column_num*sizeof(short)) >> 1) <= video_mem_end){
        video_mem_origin += scr_gap*screen_column_num*sizeof(short);
        position += scr_gap*screen_column_num*sizeof(short);
        set_origin();
        return;
    }else{
        //testPutChar(video_mem_origin,"now need move to top",strlen("now need move to top"));
        //copy bottom to the top
        int copy_size = (screen_row_num -2)*screen_column_num*sizeof(short);
        memcpy(video_mem_start, position - copy_size, copy_size);
        //clear the rest screen
        clear_screen(video_mem_start+(screen_row_num -2)*screen_column_num*sizeof(short),position);
        video_mem_origin = video_mem_start;
        position = video_mem_start+(screen_row_num -2)*screen_column_num*sizeof(short);
        set_origin();
    }
}


static void cr(void){
    position -= x*sizeof(short);
    x = 0;
}

static void lf(void){ //line feed
    if(y + 1 < bottom){
        y++;
        position += screen_column_num*sizeof(short);//(screen_column_num*2);
        return;
    }
    scrup();
}



void console_initialize(void){
    init_video_mem();
    init_screen_size();
    clear_screen(video_mem_start,video_mem_start+screen_row_num*screen_column_num*sizeof(short));
    set_cursor();
}

void console_write(unsigned char*p, int32_t size){
    int i = 0;
    for(i = 0;i < size;i++){
        char c = p[i];
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
                position -= screen_column_num*sizeof(short);
                lf();
            }
            char *tmp = position;
            *tmp = c;
            *(tmp+1) = 0x07;
            position += sizeof(short);
            x++;
            
        }
    }
    set_cursor();
    //this is for debug
    testPutValue(position-40*2,(position- video_mem_start)/(2*80),10);
}
