#ifndef KMEM_H_
#define KMEM_H_

#ifndef __SIZE_T
#define __SIZE_T
typedef unsigned int size_t;
#endif

#include "liballoc.h"

void *kmemcpy(void *to, const void *from, unsigned int n);

static inline void *kmemset(void *s, char c, unsigned int count)
{
   int d0, d1;
   __asm__ __volatile__(
   "rep\n\t"
   "stosb"
   : "=&c" (d0), "=&D" (d1)
   :"a" (c),"1" (s),"0" (count)
   :"memory");
   return s;
}

#endif /*KMEM_H_*/
