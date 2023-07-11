# mmem_debug

内存调试工具，用于检测内存泄漏，内存越界等问题。

## 1. 使用方法

在源文件中引入头文件 `mmem_debug.h`

添加编译参数 -D__mmem_debug_enbale=1

正常使用 malloc, calloc, realloc, free 等函数即可。

当不需要检测时，可以将编译参数 __mmem_debug_enbale 去掉或者设置为 0

## 2. 接口说明

``` c

/**
 * @brief mmem_calloc       calloc
 * @param counts[in]        item counts
 * @param item_size[in]     item size
 * @param file[in]          file name
 * @param line[in]          line number
 * @return (void *)         address
*/
extern void *mmem_calloc(unsigned long counts, unsigned long item_size, const char *file, int line);

/**
 * @brief mmem_alloc        malloc
 * @param size[in]          size to malloc
 * @param file[in]          file name
 * @param line[in]          line number
 * @return (void *)         address
*/
extern void *mmem_alloc(unsigned long size, const char *file, int line);

/**
 * @brief mmem_free         free
 * @param addr[in]          address to free
 * @param file[in]          file name
 * @param line[in]          line number
 * @return void
*/
extern void mmem_free(void *addr, const char *file, int line);

/**
 * @brief mmem_realloc      realloc
 * @param addr[in]          address to realloc, if NULL, it will be malloc
 * @param size[in]          size to realloc, if 0, it will be free
 * @param file[in]          file name
 * @param line[in]          line number
 * @return (void *)
*/
extern void *mmem_realloc(void *addr, unsigned long size, const char *file, int line);

```

## 3. dump内存信息

``` c

/**
 * @brief mmem_dump         dump memory
 * @param argc[in]          argument counts
 * @param argv[in]          argument values
 * @param counts[in]        item counts
 * @param buf[out]          buffer to store dump data
 * @param buf_size[in]      buffer size
 * @return (long)           dump data size
*/
extern long mmem_dump(const int argc, const char **argv, unsigned long counts, char *buf, unsigned long buf_size);

...

// example
char dump_buf[1024] = {0};
mmem_dump(0, NULL, 100, dump_buf, 1024);
printf("%s\n", dump_buf);

...

```

``` txt

// output
========= mmem_dump start =========
counts: 1, total: 1088, active: 1024
-----------------------------------
size    file
-----------------------------------
1024    /home/yhuan/workspace/mmem_debug/test/main.c:12

=========  mmem_dump end  =========


counts          : 当前申请了多少个内存块
total           : 当前申请的内存总大小(包含了内存管理结构体的大小)
active          : 当前申请的内存总大小(不包含内存管理结构体的大小)

size            : 当前内存块的大小
file            : 当前内存块的申请位置

```
