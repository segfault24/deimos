#include <kernel/string.h>

void *memset(void *s, int c, size_t n)
{
	size_t i;
	unsigned char *sp;

	sp = (unsigned char *)s;
	for(i=0; i<n; i++)
	{
		sp[i] = (unsigned char)c;
	}
	return s;
}
