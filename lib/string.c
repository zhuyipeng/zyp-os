#include <string.h>

size_t strlen ( const char* str ) {
	size_t	len=0;
    while(str[len]){
        len++;
    }
	return len;
}

void *memset(void *dest, char val, size_t count)
{
    unsigned char *temp = (unsigned char *)dest;
	for( ; count != 0; count--, temp[count] = val);
	return dest;
}

void *memcpy(void *dest, const void *src, size_t count)
{
 char *tmp = dest;
 const char *s = src;
 while (count--)
  *tmp = *s ;
 return dest;
}

