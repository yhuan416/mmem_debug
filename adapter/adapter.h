#ifndef __ADAPTER_H__
#define __ADAPTER_H__

#define _real_malloc(_size)                 malloc(_size)
#define _real_free(_addr)                   free(_addr)
#define _real_realloc(_addr, _size)         realloc(_addr, _size)
#define _real_calloc(_counts, _item_size)   calloc(_counts, _item_size)

#define MMEM_LOCK   (0x1)
#define MMEM_UNLOCK (0x2)

extern long mmem__do_lock(unsigned long release);

#endif // __ADAPTER_H__
