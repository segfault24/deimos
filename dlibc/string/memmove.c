#include <string.h>

void *memmove(void *s1, const void *s2, size_t n)
{
	size_t i;
	unsigned char *s1p, *s2p;

	s1p = (unsigned char *)s1;
	s2p = (unsigned char *)s2;
	if(s2p > s1p)
	{
		for(i=0; i<n; i++)
		{
			s1p[i] = s2p[i];
		}
	} else {
		for(i=n; i!=0; i--)
		{
			s1p[i-1] = s2p[i-1];
		}
	}
	return s1;
}
