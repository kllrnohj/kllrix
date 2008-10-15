#ifndef KMEM_H_
#define KMEM_H_

#ifndef __SIZE_T
#define __SIZE_T
typedef unsigned int size_t;
#endif

#include "liballoc.h"

void *kmemcpy(void *s1, const void *s2, size_t n);
void *kmemset ( void * ptr, int value, size_t num );

#endif /*KMEM_H_*/
