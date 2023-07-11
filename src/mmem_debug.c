#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#if defined(__mmem_debug_enbale) && (__mmem_debug_enbale == 1)

#include "mmem.h"
#define _mmem_lock()       mmem__do_lock(MMEM_LOCK)
#define _mmem_unlock()     mmem__do_lock(MMEM_UNLOCK)

#define _mmem_align(_size) (((_size) + (sizeof(long) - 1)) & (~(sizeof(long) - 1)))

#define MMEM_BLOCK_ACTIVE_HEAD_MAGIC    (*((long *)"mbah    "))
#define MMEM_BLOCK_ACTIVE_TAIL_MAGIC    (*((long *)"mbat    "))
#define MMEM_BLOCK_FREE_HEAD_MAGIC      (*((long *)"mbfh    "))
#define MMEM_BLOCK_FREE_TAIL_MAGIC      (*((long *)"mbft    "))

typedef struct mmem_block {
    unsigned long magic;
    struct mmem_block *next;
    struct mmem_block *prev;
    unsigned long size;
    const char *file;
    unsigned long line;
    char data[0];
} mmem_block_t;

#define MMEM_BLOCK_SIZE _mmem_align(sizeof(mmem_block_t))

#define _mmem_get_block(_addr) ((mmem_block_t *)((char *)(_addr) - MMEM_BLOCK_SIZE))

#define _mmem_get_block_size(block) (block->size)
#define _mmem_get_block_file(block) (block->file)
#define _mmem_get_block_line(block) (block->line)

#define _mmem_get_tail_magic(block) (*((long *)((char *)(block->data) + block->size)))

static inline long _mmem_check_block_magic_active(mmem_block_t *block)
{
    if (block->magic == MMEM_BLOCK_ACTIVE_HEAD_MAGIC) {
        if (_mmem_get_tail_magic(block) == MMEM_BLOCK_ACTIVE_TAIL_MAGIC) {
            return 0;
        }
    }
    return -1;
}

static inline long _mmem_set_block_magic_active(mmem_block_t *block)
{
    block->magic = MMEM_BLOCK_ACTIVE_HEAD_MAGIC;
    _mmem_get_tail_magic(block) = MMEM_BLOCK_ACTIVE_TAIL_MAGIC;
    return 0;
}

static inline long _mmem_check_block_magic_free(mmem_block_t *block)
{
    if (block->magic == MMEM_BLOCK_FREE_HEAD_MAGIC) {
        if (_mmem_get_tail_magic(block) == MMEM_BLOCK_FREE_TAIL_MAGIC) {
            return 0;
        }
    }
    return -1;
}

static inline long _mmem_set_block_magic_free(mmem_block_t *block)
{
    block->magic = MMEM_BLOCK_FREE_HEAD_MAGIC;
    _mmem_get_tail_magic(block) = MMEM_BLOCK_FREE_TAIL_MAGIC;
    return 0;
}

void *mmem_calloc(unsigned long counts, unsigned long item_size, const char* file, int line)
{
    printf("mmem_calloc: %s:%d\n", file, line);

    void *ret = calloc(counts, item_size);
    return ret;
}

void *mmem_alloc(unsigned long size, const char* file, int line)
{
    printf("mmem_alloc: %s:%d\n", file, line);

    unsigned long total_size = MMEM_BLOCK_SIZE + size + sizeof(long);
    mmem_block_t *block = (mmem_block_t *)malloc(total_size);
    if (block == NULL) {
        return NULL;
    }

    _mmem_lock();

    block->size = size;
    block->file = file;
    block->line = line;
    
    _mmem_set_block_magic_active(block);

    _mmem_unlock();

    return &block->data;
}

void mmem_free(void* addr, const char* file, int line)
{
    printf("mmem_free: %p:%s:%d\n", addr, file, line);

    if (addr == NULL) {
        return;
    }

    mmem_block_t *block = _mmem_get_block(addr);
    _mmem_lock();

    if (_mmem_check_block_magic_active(block)) {
        printf("mmem_free: block %p magic error!\n", block);
        _mmem_unlock();
        return;
    }

    _mmem_set_block_magic_free(block);

    _mmem_unlock();

    free(block);
}

void *mmem_realloc(void* addr, unsigned long size, const char* file, int line)
{
    void *ret = realloc(addr, size);
    return ret;
}

#endif/* defined(__mmem_debug_enbale) && (__mmem_debug_enbale == 1) */
