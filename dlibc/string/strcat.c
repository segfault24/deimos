#include <string.h>

char *strcat(char * __restrict s1, const char * __restrict s2)
{
	char *s1p, *s2p;

	s1p = s1;
	s2p = (char *)s2;
	while(*s1p != '\0')
	{
		s1p++;
	}
	while(*s2p != '\0')
	{
		*s1p = *s2p;
		s1p++;
		s2p++;
	}
	*s1p = '\0';
	return s1;
}
