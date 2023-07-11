#ifndef __MMEM_H__
#define __MMEM_H__

#define MMEM_LOCK   (0x1)
#define MMEM_UNLOCK (0x2)

#define _real_malloc    malloc
#define _real_free      free
#define _real_realloc   realloc
#define _real_calloc    calloc

long mmem__do_lock(unsigned long release);

#endif // __MMEM_H__
