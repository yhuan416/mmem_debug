#ifndef __MMEM_H__
#define __MMEM_H__

#define MMEM_LOCK   (0x1)
#define MMEM_UNLOCK (0x2)

long mmem__do_lock(unsigned long release);

#endif // __MMEM_H__
