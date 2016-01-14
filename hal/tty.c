#include "tty.h"
tty_t tty_table[5] = {0x0};

void tty_interrupt(int num){
    tty_t *tty = &tty_table[num];
    if(*tty != 0){
        
    }
}

static void read_to_rbuf(tty_t * tty, char*buf,int size)
{
    int len = 0;
    if(tty->read_size + size <= TTY_BUF_SIZE){
        len = size;
    }else{
        len = TTY_BUF_SIZE - tty->read_size;
    }
    
    
}
static void write_to_wbuf(tty_t * tty, char*buf,int size)
{
    //if(read_size)
   
    
}
void tty_initialize(void){
    tty_t t0;
    t0.write = write_to_wbuf;
    t0.read = read_to_rbuf;
    t0.read_size  = 0;
    t0.write_size = 0;
}

int tty_read(int num,char *buf,int size){
    
}

int tty_write(int num,char *buf,int size){
    
    
}