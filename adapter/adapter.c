#include <stdio.h>
#include <stdlib.h>

#include "pthread.h"

#include "adapter.h"

long mmem__do_lock(unsigned long release)
{
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    if (release == MMEM_UNLOCK) {
        pthread_mutex_unlock(&mutex);
    } else if (release == MMEM_LOCK) {
        pthread_mutex_lock(&mutex);
    } else {
        return -1;
    }
    return 0;
}
