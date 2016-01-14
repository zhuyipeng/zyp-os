#include "keyboard.h"
#include "tty.h"
#include "console.h"

#define wait() \
__asm__ ("\tjmp 1f\n" \
         "1:\tjmp 1f\n" \
         "1:")
extern void keyboard_interrupt();
void(*key_table_map[64*4])(unsigned char p) = {0x0};
unsigned char key_map[64*4]  = {
      0,   27, '1','2','3','4','5','6','7','8', //0
    '9',  '0', '-','=',127,9,'q','w','e','r', //10
    't', 'y','u','i','o','p','[',']','\n', 0,    //20  
    'a','s','d','f','g','h','j','k','l',';',                                          //30    
    '\'','`',0,'\\','z','x','c','v','b','n',                        //40
    'm',',','.','/',0,0,0,32,0,0,
};
unsigned char shift_key_map[64*4]  = {
      0,   27, '!','@','#','$','%','^','&','*', //0
    '(',  ')', '_','+',127,9,'Q','W','E','R', //10
    'T', 'Y','U','I','O','P','{','}','\n', 0,    //20  
    'A','S','D','F','G','H','J','K','L',':',                                          //30    
    '\"','~',0,'|','Z','X','C','V','B','N',                        //40
    'M','<','>','?',0,0,0,32,0,0,
};
long          mode_shared_with_interrupt = 0;
static unsigned char e0                  = 0;
static char shift_mode = 0;
static char ctrl_mode = 0;
static char alt_mode = 0;
static char caps_mode = 0;
static unsigned char keyboard_char_cache[16]={0x0};
static unsigned char keyboard_char_cache_length = 0;

void copy_to_tty_read_cache(void/*tty driver maybe here*/){
    //console_write(keyboard_char_cache,keyboard_char_cache_length);
    tty_table[0].read(&tty_table[0], keyboard_char_cache,keyboard_char_cache_length);
}

void do_self(unsigned char scan_code){
    keyboard_char_cache_length = 0;
    if(!caps_mode && !shift_mode){
        if(key_map[scan_code]){
            keyboard_char_cache[0]=key_map[scan_code];
            keyboard_char_cache_length++;
        }
    }else if(caps_mode == 1 || shift_mode == 1){
        if(shift_key_map[scan_code]){
            keyboard_char_cache[0]=shift_key_map[scan_code];
            keyboard_char_cache_length++;
        }
    }
    copy_to_tty_read_cache();
}

static void ctrl(unsigned char scan_code){
    char p="ctrl"; 
    //p[0] = key_map[scan_code];
    //testPutValue(scan_code,10);
    ctrl_mode = 1;
}

static void unctrl(unsigned char scan_code){
    ctrl_mode = 0;
}


static void lshift(unsigned char scan_code){
    //char *p="lshift"; 
    //p[0] = key_map[scan_code];
    //console_write(p,6);
    shift_mode = 1;
}

static void unlshift(unsigned char scan_code){
    //char *p="unlshift"; 
    //p[0] = key_map[scan_code];
    //console_write(p,8);
    shift_mode = 0;
}

static void minus(unsigned char scan_code){
    
}

static void rshift(unsigned char scan_code){
    shift_mode = 1;
}

static void unrshift(unsigned char scan_code){
    shift_mode = 0;
}



static void alt(unsigned char scan_code){
    alt_mode = 1;
}

static void unalt(unsigned char scan_code){
    alt_mode = 0;
}



static void caps(unsigned char scan_code){
   // char *p="caps"; 
    //p[0] = key_map[scan_code];
   // console_write(p,4);
    if(caps_mode == 0){
        caps_mode = 1;
    }else{
        caps_mode = 0;
    }
}

static void uncaps(unsigned char scan_code){
    
}

static void func(unsigned char scan_code){
    
}

static void num(unsigned char scan_code){
    
}

static void scroll(unsigned char scan_code){
    
}
static void reboot(){
    

}

static unsigned char cursor_table[13] ={'H','A'/*up*/,'5',' ','D'/*left*/,'G','C'/*right*/,0,'Y','B'/*down*/,'6','2','3'};
                                       //{'H', 'Y', 'A', 'B', 'D', 'C', 'V', 'U', 'G', 'S', 'T', '@'};
static void move_cursor(unsigned char scan_code){
    
    unsigned char p = cursor_table[scan_code - 73];
    keyboard_char_cache[0] = 0x1b;
    keyboard_char_cache[1] = 0x5b;
    keyboard_char_cache[2] = p;
    keyboard_char_cache_length = 3;
    copy_to_tty_read_cache();
}

static void cursor(unsigned char scan_code){
    return;//not suppose open yet
    if(scan_code == 83){
        if(ctrl_mode&&alt_mode){
            reboot();
            return;
        }
    }
    if(e0 == 1 || shift_mode){
        move_cursor(scan_code);
    }
    
}

//char * (*(*pf)[3])(char * p);

void keyboard_initialize(void){
    setvect (0x21, keyboard_interrupt);
    int i = 0;
    for(i = 1;i < 64*4;i++){
        //if()
        //key_table_map[i] = do_self;
        if(i <= 88){
            if(i == 58){
                key_table_map[i] = caps;
            }else if(i == 29){
                key_table_map[i] = ctrl;
            }else if(i == 42){
                key_table_map[i] = lshift;
            }else if(i == 54){
                key_table_map[i] = rshift;
            }else if(i == 56){
                key_table_map[i] = alt;
            }else if(i >= 59 && i <= 68){
                key_table_map[i] = func;
            }else if(i == 69){
                key_table_map[i] = num;//scroll
            }else if(i == 70){
                key_table_map[i] = scroll;//scroll
            }else if(i == 71){
                key_table_map[i] = cursor;//home
            }else if(i == 72){
                key_table_map[i] = cursor;//up
            }else if(i == 73){
                key_table_map[i] = cursor;//page up
            }else if(i == 74){
                key_table_map[i] = 0;//
            }else if(i == 75){
                key_table_map[i] = cursor;//left
            }else if(i == 76){
                key_table_map[i] = cursor;//n5
            }else if(i == 77){
                key_table_map[i] = cursor;//right
            }else if(i == 78){
                key_table_map[i] = 0;//right
            }else if(i == 79){
                key_table_map[i] = cursor;//end
            }else if(i == 80){
                key_table_map[i] = cursor;//down
            }else if(i == 81){
                key_table_map[i] = cursor;////page down
            }else if(i == 82){
                key_table_map[i] = cursor;//insert
            }else if(i == 83){
                key_table_map[i] = cursor;//delete
            }else if(i >= 84 && i <= 86){
                key_table_map[i] = 0;
            }else if(i == 87 || i == 88){
                key_table_map[i] = func;
            }else{
                key_table_map[i] = do_self;
            }
        }else{
            if(i == 58+0x80){
                key_table_map[i] = uncaps;
            }else if(i == 29+128){
                key_table_map[i] = unctrl;
            }else if(i == 42+128){
                key_table_map[i] = unlshift;
            }else if(i == 54+128){
                key_table_map[i] = unrshift;
            }else if(i == 56+128){
                key_table_map[i] = alt;
            }else{
                key_table_map[i] = 0;
            }
        }
    }
}

void keyboard_reset_e0_and_e1(){
    e0 = 0;
}

void keyboard_response(){
    unsigned char value = inportb(0x60);
    wait();
    
    value |= 0x80;
    wait();
    
    outportb(0x61,value);
    wait();
    
    value &= 0x7f;
    outportb(0x61,value);
    
}

void keyboard_set_e0(){
    e0  = 1;
}

void keyboard_set_e1(){
    e0 = 2;
}
void keyboard_test(void){
    char *p = "yoo~,I am testing\r\n";
    console_write(p,strlen(p));
    
}

void keyboard_scan_code(){
    unsigned char scan_code = inportb(0x60);
    testPutValue(scan_code,16);
    if(scan_code == 0xe0){
        keyboard_set_e0();
    }else if(scan_code == 0xe1){
        keyboard_set_e1();
    }else{
        keyboard_table(scan_code);
        keyboard_reset_e0_and_e1();
    }
    keyboard_response();
}

void keyboard_table(unsigned char scan_code){
    void (*pFunc)(char)  = key_table_map[scan_code];
    if(pFunc){
        pFunc(scan_code);
    }else{
        char *p = "I am a basterd:(\r\n";
        //console_write(p,strlen(p));
    }
    //ready to copy to tty driver
    
}

