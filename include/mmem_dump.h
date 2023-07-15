#ifndef __MMEM_DUMP_H__
#define __MMEM_DUMP_H__

#define MMEM_DUMP_RET_OK                    (0)
#define MMEM_DUMP_RET_INVALID_CMD           (-1)
#define MMEM_DUMP_RET_EMPTY_BUF             (-2)
#define MMEM_DUMP_RET_BUF_SIZE_TOO_SMALL    (-3)

typedef struct mmem_info {
    unsigned long counts;
    unsigned long total_size;
    unsigned long active_size;
    unsigned long max_total_size;
    unsigned long max_active_size;
} mmem_info_t;

// dump command
#define MMEM_DUMP_CMD_COUNTS     (0x01)
#define MMEM_DUMP_CMD_MMEM_INFO       (0x02)

/**
 * @brief mmem_dump         dump memory info
 * @param cmd[in]           command
 * @param buf[out]          buffer to store dump data
 * @param buf_size[in]      buffer size
 * @return (long)           0: success, others: fail
*/
extern long mmem_dump(unsigned long cmd, void *buf, unsigned long buf_size);

#endif // !__MMEM_DUMP_H__
