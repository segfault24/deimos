#include <string.h>

int memcmp(const void *s1, const void *s2, size_t n)
{
	size_t i;
	unsigned char *s1p, *s2p;

	s1p = (unsigned char *)s1;
	s2p = (unsigned char *)s2;
	for(i=0; i<n; i++)
	{
		if(s1p[i] == s2p[i])
		{
			continue;
		} else if (s1p[i] > s2p[i]) {
			return 1;
		} else {
			return -1;
		}
	}
	return 0;
}
