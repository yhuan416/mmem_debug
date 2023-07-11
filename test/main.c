#include <stdio.h>
#include <stdlib.h>

#include "mmem_debug.h"

int main(int argc, char const *argv[])
{
    char dump_buf[1024] = {0};

    printf("Hello world!\n");

    char *buf = (char *)malloc(1024);
    if (buf == NULL) {
        printf("malloc failed!\n");
        return -1;
    }

    printf("malloc success!, %p\n", buf);

    mmem_dump(argc, argv, 100, dump_buf, 1024);
    printf("%s\n", dump_buf);

    free(buf);

    mmem_dump(argc, argv, 100, dump_buf, 1024);
    printf("%s\n", dump_buf);

    return 0;
}
