#ifndef _TTY_H
#define _TTY_H
#define TTY_BUF_SIZE 1024
typedef struct tty {
    void (write*)(struct tty* t,char*buf,int size);
    void (read*)(struct tty* t, char*buf,int size);
    char read_buf[TTY_BUF_SIZE];
    int read_size;
    char write_buf[TTY_BUF_SIZE];
    int write_size;
}tty_t;

void tty_initialize(void);

int tty_read(int num,char *buf,int size);//read from tty terminal means cin:)

int tty_write(int num,char *buf,int size);//write to tty terminal,means cout:)

#endif