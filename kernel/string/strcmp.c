#include <kernel/string.h>

int strcmp(const char *s1, const char *s2)
{
	char *s1p, *s2p;

	s1p = (char *)s1;
	s2p = (char *)s2;
	while(*s1p != '\0' && *s2p != '\0' && *s1p == *s2p)
	{
		s1p++;
		s2p++;
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
