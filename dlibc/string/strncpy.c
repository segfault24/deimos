#include <string.h>

char *strncpy(char * __restrict s1, const char * __restrict s2, size_t n)
{
	size_t i;
	char *s1p, *s2p;

	s1p = s1;
	s2p = (char *)s2;
	for(i=0; *s2p!='\0' && i<n; i++, s2p++, s1p++)
	{
		*s1p = *s2p;
	}
	for(; i<n; i++, s1p++)
	{
		*s1p = '\0';
	}
	return s1;
}
