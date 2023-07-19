#include <stdio.h>
#include <stdlib.h>

#include "pthread.h"
#include <stdarg.h>

#include "adapter.h"

long mmem_lock(unsigned long lock)
{
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    if (lock == MMEM_UNLOCK) {
        pthread_mutex_unlock(&mutex);
    } else if (lock == MMEM_LOCK) {
        pthread_mutex_lock(&mutex);
    } else {
        return -1;
    }
    return 0;
}

#define MMEM_PRINT_BUF_SIZE (1024)

int mmem_printf(const int level, const char *fmt, ...)
{
    int ret;
    va_list args;
    char print_buf[MMEM_PRINT_BUF_SIZE] = {0};

    va_start(args, fmt);
    ret = vsnprintf(print_buf, MMEM_PRINT_BUF_SIZE, fmt, args);
    va_end(args);

    switch (level)
    {
    case MMEM_LEVEL_ERROR:
        printf("\n[ERROR] %s.\n", print_buf);
        break;
    case MMEM_LEVEL_DEBUG:
        printf("\n[DEBUG] %s.\n", print_buf);
        break;
    default:
        break;
    }

    return 0;
}
