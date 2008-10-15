#include "kmem.h"

void *kmemcpy(void *s1, const void *s2, size_t n)
{
	char *c1 = (char*)s1;
	char *c2 = (char*)s2;
	size_t i;
	for (i = 0; i < n; i++)
	{
		*c1 = *c2;
		c1++;
		c2++;
	}
	return s1;
}

void *kmemset ( void * ptr, int value, size_t num )
{
	char* cptr = (char*)ptr;
	size_t i;
	for (i = 0; i < num; i++)
	{
		*cptr = value;
		cptr++;
	}
	return ptr;
}
