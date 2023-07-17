#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "mlist.h"

#include "mmem_dump.h"

#include "mmem_debug_adapter.h"
#define _mmem_lock()       do { mmem__do_lock(MMEM_LOCK); } while(0)
#define _mmem_unlock()     do { mmem__do_lock(MMEM_UNLOCK); } while(0)

#define _align(_size, _align) (((_size) + ((_align) - 1)) & (~((_align) - 1)))

#define _mmem_align(_size) _align(_size, sizeof(long))

typedef struct mmem_block {
    unsigned long magic;            // 头部魔数
    MLIST list;                     // 链表节点
    unsigned long size;             // 数据域大小
    unsigned long total_size;       // 总大小
    const char *file;               // 文件名
    unsigned long line;             // 行号
    char data[0];                   // 数据域变长数组
} mmem_block_t;

#define MMEM_BLOCK_SIZE                     _mmem_align(sizeof(mmem_block_t))
#define _mmem_total_size(_size)             (unsigned long)(MMEM_BLOCK_SIZE + (_size) + sizeof(long))

#define _mmem_get_block(_addr)              ((mmem_block_t *)((char *)(_addr) - MMEM_BLOCK_SIZE))

#define _mmem_block_magic(_block)           ((_block)->magic)
#define _mmem_block_size(_block)            ((_block)->size)
#define _mmem_block_total_size(_block)      ((_block)->total_size)
#define _mmem_block_file(_block)            ((_block)->file)
#define _mmem_block_line(_block)            ((_block)->line)
#define _mmem_block_data(_block)            (void *)(&((_block)->data))
#define _mmem_block_tail_magic(_block)      (*((long *)((char *)((_block)->data) + (_block)->size)))

#define _mmem_block_update(_block, _size, _total_size, _file, _line) \
    do { \
        _mmem_block_size(_block) = (_size); \
        _mmem_block_total_size(_block) = (_total_size); \
        _mmem_block_file(_block) = (_file); \
        _mmem_block_line(_block) = (_line); \
    } while(0)

#define MMEM_BLOCK_ACTIVE_HEAD_MAGIC    (*((long *)"mbah    "))
#define MMEM_BLOCK_ACTIVE_TAIL_MAGIC    (*((long *)"mbat    "))
#define MMEM_BLOCK_FREE_HEAD_MAGIC      (*((long *)"mbfh    "))
#define MMEM_BLOCK_FREE_TAIL_MAGIC      (*((long *)"mbft    "))

#define _mmem_check_block_magic_active(_block) \
    ((_mmem_block_magic(_block) != MMEM_BLOCK_ACTIVE_HEAD_MAGIC || \
        _mmem_block_tail_magic(_block) != MMEM_BLOCK_ACTIVE_TAIL_MAGIC) ? -1 : 0)

#define _mmem_set_block_magic_active(_block) \
    do { \
        _mmem_block_magic(_block) = MMEM_BLOCK_ACTIVE_HEAD_MAGIC; \
        _mmem_block_tail_magic(_block) = MMEM_BLOCK_ACTIVE_TAIL_MAGIC; \
    } while(0)

#define _mmem_set_block_magic_free(_block) \
    do { \
        _mmem_block_magic(_block) = MMEM_BLOCK_FREE_HEAD_MAGIC; \
        _mmem_block_tail_magic(_block) = MMEM_BLOCK_FREE_TAIL_MAGIC; \
    } while (0)

typedef struct mmem_block_table {
    unsigned long count;
    MLIST list;
    unsigned long total_size;
    unsigned long active_size;
    unsigned long max_total_size;
    unsigned long max_active_size;
} mmem_block_table_t;

static mmem_block_table_t mmem_block_table = {0};

static mmem_block_table_t *_mmem_block_table_get(void)
{
    static mmem_block_table_t *table = NULL;

    if (table == NULL) {
        table = &mmem_block_table;

        table->total_size = 0;
        table->active_size = 0;

        // init list head
        MLIST_INIT(&(table->list));
    }

    return table;
}

#define _mmem_block_add(_table, _block) \
    do { \
        MLIST_ADD_TAIL(&((_table)->list), &((_block)->list)); \
        (_table)->count++; \
        (_table)->total_size += (_block)->total_size; \
        (_table)->active_size += (_block)->size; \
        if ((_table)->active_size > (_table)->max_active_size) { \
            (_table)->max_active_size = (_table)->active_size; \
        } \
        if ((_table)->total_size > (_table)->max_total_size) { \
            (_table)->max_total_size = (_table)->total_size; \
        } \
    } while(0)

#define _mmem_block_del(_table, _block) \
    do { \
        MLIST_DEL(&((_block)->list)); \
        (_table)->count--; \
        (_table)->total_size -= (_block)->total_size; \
        (_table)->active_size -= (_block)->size; \
    } while(0)


void *mmem_calloc(unsigned long counts, unsigned long item_size, const char* file, int line)
{
    mmem_block_t *block = NULL;
    mmem_block_table_t *table = NULL;
    unsigned long size = 0;
    unsigned long total_size = 0;

    if (counts == 0 || item_size == 0) {
        return NULL;
    }

    size = counts * item_size;
    total_size = _mmem_total_size(size);
    block = (mmem_block_t *)_real_calloc(1, total_size);
    if (block == NULL) {
        return NULL;
    }

    _mmem_lock();

    table = _mmem_block_table_get();

    _mmem_block_update(block, size, total_size, file, line);

    _mmem_set_block_magic_active(block);

    MLIST_INIT(&(block->list));

    // add to block table
    _mmem_block_add(table, block);

    _mmem_unlock();

    return _mmem_block_data(block);
}


void *mmem_alloc(unsigned long size, const char* file, int line)
{
    mmem_block_t *block = NULL;
    mmem_block_table_t *table = NULL;
    unsigned long total_size;

    if (size == 0) {
        return NULL;
    }

    total_size = _mmem_total_size(size);
    block = (mmem_block_t *)_real_malloc(total_size);
    if (block == NULL) {
        return NULL;
    }

    _mmem_lock();

    table = _mmem_block_table_get();

    // update block info
    _mmem_block_update(block, size, total_size, file, line);

    // set block magic
    _mmem_set_block_magic_active(block);

    MLIST_INIT(&(block->list));

    // add to block table
    _mmem_block_add(table, block);

    _mmem_unlock();

    return _mmem_block_data(block);
}


void mmem_free(void* addr, const char* file, int line)
{
    mmem_block_t *block = NULL;
    mmem_block_table_t *table = NULL;

    if (addr == NULL) {
        return;
    }

    _mmem_lock();

    table = _mmem_block_table_get();

    block = _mmem_get_block(addr);

    if (_mmem_check_block_magic_active(block)) {
        printf("mmem_free: block %p magic error!\n", block);
        _mmem_unlock();
        return;
    }

    // set block magic
    _mmem_set_block_magic_free(block);

    // delete old block
    _mmem_block_del(table, block);

    _mmem_unlock();

    _real_free(block);
}


void *mmem_realloc(void* addr, unsigned long size, const char* file, int line)
{
    mmem_block_t *block = NULL;
    mmem_block_t *new_block = NULL;
    mmem_block_table_t *table = NULL;
    unsigned long total_size;

    // if addr is NULL, realloc is equal to malloc
    if (addr == NULL) {
        return mmem_alloc( size, file, line );
    }

    // if size is 0, realloc is equal to free
    if (size == 0) {
        mmem_free(addr, file, line);
        return NULL;
    }

    _mmem_lock();

    table = _mmem_block_table_get();

    block = _mmem_get_block(addr);

    if (_mmem_check_block_magic_active(block)) {
        printf("mmem_realloc: block %p magic error!\n", block);
        _mmem_unlock();
        return NULL;
    }

    // set old block magic to free
    _mmem_set_block_magic_free(block);

    // delete old block
    _mmem_block_del(table, block);

    // alloc new block
    total_size = _mmem_total_size(size);
    new_block = (mmem_block_t *)_real_realloc(block, total_size);
    if (new_block == NULL) {
        printf("mmem_realloc: realloc failed!\n");
        _mmem_unlock();
        return NULL;
    }

    block = new_block;

    // update new block
    _mmem_block_update(block, size, total_size, file, line);

    // set new block magic
    _mmem_set_block_magic_active(block);

    MLIST_INIT(&(block->list));

    // add to block table
    _mmem_block_add(table, block);

    _mmem_unlock();

    return _mmem_block_data(block);
}

// mmem_dump
static long _mmem_dump_cmd_counts(mmem_block_table_t *table, char *buf, unsigned long buf_size);
static long _mmem_dump_cmd_mmem_info(mmem_block_table_t *table, char *buf, unsigned long buf_size);
static long _mmem_dump_cmd_mmem_block_info(mmem_block_table_t *table, unsigned long counts, char *buf, unsigned long buf_size);

long mmem_dump(unsigned long cmd, unsigned long counts, void *buf, unsigned long buf_size)
{
    long ret = 0;
    mmem_block_table_t *table = NULL;

    _mmem_lock();

    table = _mmem_block_table_get();

    switch (cmd)
    {
    case MMEM_DUMP_CMD_COUNTS:
        ret = _mmem_dump_cmd_counts(table, buf, buf_size);
        break;

    case MMEM_DUMP_CMD_MMEM_INFO:
        ret = _mmem_dump_cmd_mmem_info(table, buf, buf_size);
        break;
    
    case MMEM_DUMP_CMD_MMEM_BLOCK_INFO:
        ret = _mmem_dump_cmd_mmem_block_info(table, counts, buf, buf_size);
        break;

    default:
        // printf("mmem_dump: invalid cmd %ld\n", cmd);
        ret = MMEM_DUMP_RET_INVALID_CMD;
        break;
    }

    _mmem_unlock();

    return ret;
}

// mmem_free_all
void mmem_free_all(void)
{
    mmem_block_t *block = NULL, *n;
    mmem_block_table_t *table = NULL;

    _mmem_lock();

    table = _mmem_block_table_get();

    MLIST_FOR_EACH_ENTRY_SAFE(block, n, mmem_block_t, &(table->list), list) {

        // check block magic
        if (_mmem_check_block_magic_active(block)) {
            printf("mmem_free_all: block %p magic error!\n", block);
        }

        // set block magic
        _mmem_set_block_magic_free(block);

        // delete old block
        _mmem_block_del(table, block);

        // free block
        _real_free(block);
    }

    _mmem_unlock();
}

// mmem_dump cmd
static long _mmem_dump_cmd_counts(mmem_block_table_t *table, char *buf, unsigned long buf_size)
{
    long ret = MMEM_DUMP_RET_OK;

    if (buf == NULL) {
        return MMEM_DUMP_RET_EMPTY_BUF;
    }

    if (buf_size < sizeof(table->count)) {
        return MMEM_DUMP_RET_BUF_SIZE_TOO_SMALL;
    }

    *((unsigned long *)buf) = table->count;

    return ret;
}

static long _mmem_dump_cmd_mmem_info(mmem_block_table_t *table, char *buf, unsigned long buf_size)
{
    long ret = MMEM_DUMP_RET_OK;

    if (buf == NULL) {
        return MMEM_DUMP_RET_EMPTY_BUF;
    }

    if (buf_size < sizeof(mmem_info_t)) {
        return MMEM_DUMP_RET_BUF_SIZE_TOO_SMALL;
    }

    mmem_info_t *info = (mmem_info_t *)buf;

    info->counts = table->count;
    info->total_size = table->total_size;
    info->active_size = table->active_size;
    info->max_total_size = table->max_total_size;
    info->max_active_size = table->max_active_size;

    return ret;
}

static long _mmem_dump_cmd_mmem_block_info(mmem_block_table_t *table, unsigned long counts, char *buf, unsigned long buf_size)
{
    long index = 0;
    long ret = MMEM_DUMP_RET_OK;
    mmem_block_t *block = NULL;
    unsigned long size = buf_size;
    mmem_block_info_t *info = (mmem_block_info_t *)buf;

    // check buf
    if (buf == NULL) {
        return MMEM_DUMP_RET_EMPTY_BUF;
    }

    // check buf size
    if (buf_size < sizeof(mmem_block_info_t)) {
        return MMEM_DUMP_RET_BUF_SIZE_TOO_SMALL;
    }

    if (counts == 0) {
        return MMEM_DUMP_RET_OK;
    }

    MLIST_FOR_EACH_ENTRY(block, mmem_block_t, &(table->list), list) {

        info[index].size = _mmem_block_size(block);
        info[index].total_size = _mmem_block_total_size(block);
        info[index].file = _mmem_block_file(block);
        info[index].line = _mmem_block_line(block);
        index++;

        // check left counts
        if (index >= counts) {
            break;
        }

        // check left buf size
        size -= sizeof(mmem_block_info_t);
        if (size < sizeof(mmem_block_info_t)) {
            break;
        }
    }

    return index;
}
