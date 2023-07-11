#include <stdio.h>
#include <stdlib.h>

#include "mmem_debug.h"

int main(int argc, char const *argv[])
{
    printf("Hello world!\n");

    char *buf = (char *)malloc(1024);
    if (buf == NULL) {
        printf("malloc failed!\n");
        return -1;
    }

    printf("malloc success!, %p\n", buf);

    free(buf);

    return 0;
}
