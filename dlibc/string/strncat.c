#include <string.h>

char *strncat(char * __restrict s1, const char * __restrict s2, size_t n)
{
	size_t i;
	char *s1p, *s2p;

	s1p = s1;
	s2p = (char *)s2;
	while(*s1p != '\0')
	{
		s1p++;
	}
	i = 0;
	while(*s2p != '\0' && i<n)
	{
		*s1p = *s2p;
		s1p++;
		s2p++;
		i++;
	}
	*s1p = '\0';
	return s1;
}
