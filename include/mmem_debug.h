#ifndef __MMEM_DEBUG_H__
#define __MMEM_DEBUG_H__

#include <stdlib.h>

#if defined(__cplusplus)
extern "C" {
#endif/* defined(__cplusplus) */

#if defined(__mmem_debug_enbale) && (__mmem_debug_enbale == 1)

#   undef calloc
#   define calloc(_counts, _item_size)  mmem_calloc(_counts, _item_size, __FILE__, __LINE__)
#   undef malloc
#   define malloc(_size)                mmem_alloc(_size, __FILE__, __LINE__)
#   undef free
#   define free(_addr)                  mmem_free(_addr, __FILE__, __LINE__)
#   undef realloc
#   define realloc(_addr, _size)        mmem_realloc(_addr, _size, __FILE__, __LINE__)

/**
 * @brief mmem_calloc
 * @param counts
 * @param item_size
 * @param file
 * @param line
 * @return
*/
extern void *mmem_calloc(unsigned long counts, unsigned long item_size, const char *file, int line);

/**
 * @brief mmem_alloc
 * @param size
 * @param file
 * @param line
 * @return
*/
extern void *mmem_alloc(unsigned long size, const char *file, int line);

/**
 * @brief mmem_free
 * @param addr
 * @param file
 * @param line
 * @return void
*/
extern void mmem_free(void *addr, const char *file, int line);

/**
 * @brief mmem_realloc
 * @param addr
 * @param size
 * @param file
 * @param line
 * @return
*/
extern void *mmem_realloc(void *addr, unsigned long size, const char *file, int line);

#endif/* defined(__mmem_debug_enbale) && (__mmem_debug_enbale == 1) */

#if defined(__cplusplus)
}
#endif/* defined(__cplusplus) */

#endif // !__MMEM_DEBUG_H__