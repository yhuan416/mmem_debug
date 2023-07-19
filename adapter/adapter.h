#ifndef __ADAPTER_H__
#define __ADAPTER_H__

#define _real_malloc(_size)                 malloc(_size)
#define _real_free(_addr)                   free(_addr)
#define _real_realloc(_addr, _size)         realloc(_addr, _size)
#define _real_calloc(_counts, _item_size)   calloc(_counts, _item_size)

#define MMEM_LOCK   (0x1)
#define MMEM_UNLOCK (0x2)

extern long mmem_lock(unsigned long lock);

#define MMEM_LEVEL_ERROR    (0x1)
#define MMEM_LEVEL_DEBUG    (0x2)

#define mmem_error(fmt, ...)   mmem_printf(MMEM_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define mmem_debug(fmt, ...)   mmem_printf(MMEM_LEVEL_DEBUG, fmt, ##__VA_ARGS__)

extern int mmem_printf(int level, const char *fmt, ...);

#endif // __ADAPTER_H__
