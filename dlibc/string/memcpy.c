#include <string.h>

void *memcpy(void * __restrict s1, const void * __restrict s2, size_t n)
{
	size_t i;
	unsigned char *s1p, *s2p;

	s1p = (unsigned char *)s1;
	s2p = (unsigned char *)s2;
	for(i=0; i<n; i++)
		s1p[i] = s2p[i];
	return s1;
}
