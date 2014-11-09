#include <kernel/string.h>

int strncmp(const char *s1, const char *s2, size_t n)
{
	size_t i;
	char *s1p, *s2p;

	i = 0;
	s1p = (char *)s1;
	s2p = (char *)s2;
	while(*s1p != '\0' && *s2p != '\0' && *s1p == *s2p && i<n-1)
	{
		s1p++;
		s2p++;
		i++;
	}
	if(*s1p == *s2p)
	{
		return 0;
	} else if(*s1p > *s2p) {
		return 1;
	} else {
		return -1;
	}
}
