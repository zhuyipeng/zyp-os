#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include "testDisplay.h"

#define VID_MEMORY	0xB8000
uint16_t *video_memory = (uint16_t *)0xB8000;
static unsigned int _xPos=0, _yPos=0;

static unsigned _startX=0, _startY=0;

uint8_t cursor_x = 0;
uint8_t cursor_y = 0;
uint8_t _color=0;
void DebugPutc (unsigned char c) {
    
    uint16_t attribute = _color << 8;

    if (c == 0x08 && cursor_x)
        cursor_x--;

    else if (c == 0x09)
        cursor_x = (cursor_x+8) & ~(8-1);

    else if (c == '\r')
        cursor_x = 0;

        else if (c == '\n') {
        	cursor_x = 0;
        	cursor_y++;
		if(cursor_y >= 25){
			cursor_y = 0;
		}
        }

    else if(c >= ' ') {

        uint16_t* location = video_memory + (cursor_y*80 + cursor_x);
        *location = c | attribute;
        cursor_x++;
    }

    if (cursor_x >= 80) {

        cursor_x = 0;
        cursor_y++;
	if(cursor_y >= 25){
		cursor_y = 0;
	}
    }

}
char tbuf[32];

char bchars[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void itoa(unsigned i,unsigned base,char* buf) {

   int pos = 0;

   int opos = 0;

   int top = 0;



   if (i == 0 || base > 16) {

      buf[0] = '0';

      buf[1] = '\0';

      return;

   }



   while (i != 0) {

      tbuf[pos] = bchars[i % base];

      pos++;

      i /= base;

   }

   top=pos--;

   for (opos=0; opos<top; pos--,opos++) {

      buf[opos] = tbuf[pos];

   }

   buf[opos] = 0;

}

void itoa_s(int i,unsigned base,char* buf) {

   if (base > 16) return;

   if (i < 0) {

      *buf++ = '-';

      i *= -1;

   }

   itoa(i,base,buf);

}

unsigned DebugSetColor (const unsigned c) {



	unsigned t=_color;

	_color=c;

	return t;

}

void DebugGotoXY (unsigned x, unsigned y) {



	_xPos = x*2;

	_yPos = y*2;

	_startX=_xPos;

	_startY=_yPos;

}

void DebugClrScr (const unsigned short c) {
	unsigned char* p = (unsigned char*)VID_MEMORY;
	int i = 0;
	for (i=0; i<160*30; i+=2) {
		p[i] = ' '; 
		p[i+1] = c;
	}
	_xPos=_startX;_yPos=_startY;

}

void DebugPuts (char* str) {
	if (!str)
		return;
size_t i = 0;
	for (i=0; i<strlen (str); i++)
		DebugPutc (str[i]);

}

int DebugPrintf (const char* str, ...) {
	
	if(!str)
		return 0;
	va_list		args;
	size_t i;
	va_start (args, str);
	for (i=0; i<strlen(str);i++) {
		switch (str[i]) {
			case '%':
				switch (str[i+1]) {
					case 'c': {
						char c = va_arg (args, char);
						DebugPutc (c);
						i++;		
						break;
					}
					case 's': {
						int c = (int) va_arg (args, char);
						char str[32]={0};
						itoa_s (c, 16, str);
						DebugPuts (str);
						i++;		
						break;
					}

					case 'd':

					case 'i': {

						int c = va_arg (args, int);

						char str[32]={0};

						itoa_s (c, 10, str);

						DebugPuts (str);

						i++;		

						break;

					}




					case 'X':

					case 'x': {

						int c = va_arg (args, int);

						char str[32]={0};

						itoa_s (c,16,str);

						DebugPuts (str);

						i++;		

						break;

					}



					default:

						va_end (args);

						return 1;

				}



				break;



			default:

				DebugPutc (str[i]);

				break;

		}



	}
	va_end (args);
}
