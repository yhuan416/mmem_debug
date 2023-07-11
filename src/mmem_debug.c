#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#if defined(__mmem_debug_enbale) && (__mmem_debug_enbale == 1)

#include "mmem.h"
#define _mmem_lock()       mmem__do_lock(MMEM_LOCK)
#define _mmem_unlock()     mmem__do_lock(MMEM_UNLOCK)

#ifdef _align
#undef _align
#endif

#define _align(_size, _align) (((_size) + ((_align) - 1)) & (~((_align) - 1)))

#define _mmem_align(_size) _align(_size, sizeof(long))

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

#define _mmem_block_magic(_block)        ((_block)->magic)
#define _mmem_block_size(_block)         ((_block)->size)
#define _mmem_block_file(_block)         ((_block)->file)
#define _mmem_block_line(_block)         ((_block)->line)
#define _mmem_block_data(_block)         (void *)(&((_block)->data))
#define _mmem_block_tail_magic(_block)   (*((long *)((char *)((_block)->data) + (_block)->size)))

#define _mmem_total_size(_size) (unsigned long)(MMEM_BLOCK_SIZE + (_size) + sizeof(long))

#define MMEM_BLOCK_ACTIVE_HEAD_MAGIC    (*((long *)"mbah    "))
#define MMEM_BLOCK_ACTIVE_TAIL_MAGIC    (*((long *)"mbat    "))
#define MMEM_BLOCK_FREE_HEAD_MAGIC      (*((long *)"mbfh    "))
#define MMEM_BLOCK_FREE_TAIL_MAGIC      (*((long *)"mbft    "))

static inline long _mmem_check_block_magic_active(mmem_block_t *block)
{
    if (_mmem_block_magic(block) != MMEM_BLOCK_ACTIVE_HEAD_MAGIC ||
            _mmem_block_tail_magic(block) != MMEM_BLOCK_ACTIVE_TAIL_MAGIC) {
        return -1;
    }
    return 0;
}

static inline long _mmem_set_block_magic_active(mmem_block_t *block)
{
    _mmem_block_magic(block) = MMEM_BLOCK_ACTIVE_HEAD_MAGIC;
    _mmem_block_tail_magic(block) = MMEM_BLOCK_ACTIVE_TAIL_MAGIC;
    return 0;
}

static inline long _mmem_check_block_magic_free(mmem_block_t *block)
{
    if (_mmem_block_magic(block) != MMEM_BLOCK_FREE_HEAD_MAGIC ||
            _mmem_block_tail_magic(block) != MMEM_BLOCK_FREE_TAIL_MAGIC) {
        return -1;
    }
    return 0;
}

static inline long _mmem_set_block_magic_free(mmem_block_t *block)
{
    _mmem_block_magic(block) = MMEM_BLOCK_FREE_HEAD_MAGIC;
    _mmem_block_tail_magic(block) = MMEM_BLOCK_FREE_TAIL_MAGIC;
    return 0;
}

void *mmem_calloc(unsigned long counts, unsigned long item_size, const char* file, int line)
{
    mmem_block_t *block = NULL;
    unsigned long size = counts * item_size;
    unsigned long total_size;

    printf("mmem_calloc: %s:%d\n", file, line);

    if (counts == 0 || item_size == 0) {
        return NULL;
    }

    total_size = _mmem_total_size(size);
    block = (mmem_block_t *)_real_calloc(1, total_size);
    if (block == NULL) {
        return NULL;
    }

    _mmem_lock();

    _mmem_block_size(block) = size;
    _mmem_block_file(block) = file;
    _mmem_block_line(block) = line;

    _mmem_set_block_magic_active(block);

    _mmem_unlock();

    return _mmem_block_data(block);
}

void *mmem_alloc(unsigned long size, const char* file, int line)
{
    mmem_block_t *block = NULL;
    unsigned long total_size;

    printf("mmem_alloc: %s:%d\n", file, line);

    total_size = _mmem_total_size(size);
    block = (mmem_block_t *)_real_malloc(total_size);
    if (block == NULL) {
        return NULL;
    }

    _mmem_lock();

    _mmem_block_size(block) = size;
    _mmem_block_file(block) = file;
    _mmem_block_line(block) = line;

    _mmem_set_block_magic_active(block);

    _mmem_unlock();

    return _mmem_block_data(block);
}

void mmem_free(void* addr, const char* file, int line)
{
    mmem_block_t *block = NULL;

    printf("mmem_free: %p:%s:%d\n", addr, file, line);

    if (addr == NULL) {
        return;
    }

    _mmem_lock();

    block = _mmem_get_block(addr);

    if (_mmem_check_block_magic_active(block)) {
        printf("mmem_free: block %p magic error!\n", block);
        _mmem_unlock();
        return;
    }

    _mmem_set_block_magic_free(block);

    _mmem_unlock();

    _real_free(block);
}

void *mmem_realloc(void* addr, unsigned long size, const char* file, int line)
{
    mmem_block_t *block = NULL;
    mmem_block_t *new_block = NULL;
    unsigned long total_size;

    if( addr == NULL ) {
        return mmem_alloc( size, file, line );
    }

    _mmem_lock();

    block = _mmem_get_block(addr);

    if (_mmem_check_block_magic_active(block)) {
        printf("mmem_realloc: block %p magic error!\n", block);
        _mmem_unlock();
        return NULL;
    }

    // delete old block
    _mmem_set_block_magic_free(block);


    // alloc new block
    total_size = _mmem_total_size(size);
    new_block = (mmem_block_t *)_real_realloc(block, total_size);
    if (new_block == NULL) {
        _mmem_unlock();
        return NULL;
    }

    block = new_block;

    // update new block
    _mmem_block_size(block) = size;
    _mmem_block_file(block) = file;
    _mmem_block_line(block) = line;

    _mmem_set_block_magic_active(block);

    _mmem_unlock();

    return _mmem_block_data(block);
}

#endif/* defined(__mmem_debug_enbale) && (__mmem_debug_enbale == 1) */
