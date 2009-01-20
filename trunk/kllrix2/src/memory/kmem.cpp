#include "kmem.h"

void *old_kmemcpy(void *s1, const void *s2, size_t n) {
	char *c1 = (char*) s1;
	char *c2 = (char*) s2;
	size_t i;
	for (i = 0; i < n; i++) {
		*c1 = *c2;
		c1++;
		c2++;
	}
	return s1;
}
void *kmemcpy(void *to, const void *from, unsigned int n) {

	unsigned int i;
	for (i = 0; i < n / 16; i++) {
		__asm__ __volatile__ ("movups (%0), %%xmm0\n" "movntdq %%xmm0, (%1)\n"::"r"(from), "r"(to) : "memory");

		from = (void*) ( (char*)from + 16 );
		to = (void*) ( (char*)to + 16 );
	}
	if (n & 7) {
		n = n & 7;

		int d0, d1, d2;
		__asm__ __volatile__(
				"rep ; movsl\n\t"
				"testb $2,%b4\n\t"
				"je 1f\n\t"
				"movsw\n"
				"1:\ttestb $1,%b4\n\t"
				"je 2f\n\t"
				"movsb\n"
				"2:"
				: "=&c" (d0), "=&D" (d1), "=&S" (d2)
				:"0" (n/4), "q" (n),"1" ((long) to),"2" ((long) from)
				: "memory");
	}
	return (to);
}

