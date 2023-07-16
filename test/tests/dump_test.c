#include "tests.h"

#define dump_test1_description "arg test"
void dump_test1(void)
{
    long ret = 0;
    unsigned long counts = 0;

    // should return MMEM_DUMP_RET_OK
    ret = mmem_dump(MMEM_DUMP_CMD_COUNTS, (void *)&counts, sizeof(counts));
    CU_ASSERT_EQUAL(ret, MMEM_DUMP_RET_OK);
    CU_ASSERT_EQUAL(counts, 0);

    // INVALID CMD, should return MMEM_DUMP_RET_INVALID_CMD
    ret = mmem_dump(0, (void *)&counts, sizeof(counts));
    CU_ASSERT_EQUAL(ret, MMEM_DUMP_RET_INVALID_CMD);

    // INVALID BUF, should return MMEM_DUMP_RET_EMPTY_BUF
    ret = mmem_dump(MMEM_DUMP_CMD_COUNTS, NULL, sizeof(counts));
    CU_ASSERT_EQUAL(ret, MMEM_DUMP_RET_EMPTY_BUF);

    // INVALID BUF_SIZE, should return MMEM_DUMP_RET_BUF_SIZE_TOO_SMALL
    ret = mmem_dump(MMEM_DUMP_CMD_COUNTS, (void *)&counts, 0);
    CU_ASSERT_EQUAL(ret, MMEM_DUMP_RET_BUF_SIZE_TOO_SMALL);
}

#define dump_test2_description "cmd: MMEM_DUMP_CMD_COUNTS, get block counts"
void dump_test2(void)
{
    long ret = 0;
    char *buffer = NULL, *buffer2 = NULL;
    unsigned long counts = 0;

    // before malloc, counts should be 0
    ret = mmem_dump(MMEM_DUMP_CMD_COUNTS, (void *)&counts, sizeof(counts));
    CU_ASSERT_EQUAL(ret, MMEM_DUMP_RET_OK);
    CU_ASSERT_EQUAL(counts, 0);

    // malloc a 10 bytes buffer
    buffer = malloc(10);
    CU_ASSERT_PTR_NOT_NULL(buffer);

    // after malloc, counts should be 1
    ret = mmem_dump(MMEM_DUMP_CMD_COUNTS, (void *)&counts, sizeof(counts));
    CU_ASSERT_EQUAL(ret, MMEM_DUMP_RET_OK);
    CU_ASSERT_EQUAL(counts, 1);

    // malloc a 20 bytes buffer
    buffer2 = malloc(20);
    CU_ASSERT_PTR_NOT_NULL(buffer2);

    // after malloc, counts should be 2
    ret = mmem_dump(MMEM_DUMP_CMD_COUNTS, (void *)&counts, sizeof(counts));
    CU_ASSERT_EQUAL(ret, MMEM_DUMP_RET_OK);
    CU_ASSERT_EQUAL(counts, 2);

    // free buffer
    free(buffer2);
    free(buffer);

    // after free, counts should be 0
    ret = mmem_dump(MMEM_DUMP_CMD_COUNTS, (void *)&counts, sizeof(counts));
    CU_ASSERT_EQUAL(ret, MMEM_DUMP_RET_OK);
    CU_ASSERT_EQUAL(counts, 0);
}

#define dump_test3_description "cmd: MMEM_DUMP_CMD_COUNTS, get block counts with realloc"
void dump_test3(void)
{
    long ret = 0;
    unsigned long counts = 0;
    char *buffer = NULL;

    // malloc a 10 bytes buffer
    buffer = malloc(10);
    CU_ASSERT_PTR_NOT_NULL(buffer);

    // after malloc, counts should be 1
    ret = mmem_dump(MMEM_DUMP_CMD_COUNTS, (void *)&counts, sizeof(counts));
    CU_ASSERT_EQUAL(ret, MMEM_DUMP_RET_OK);
    CU_ASSERT_EQUAL(counts, 1);

    // realloc buffer to 20 bytes
    buffer = realloc(buffer, 20);
    CU_ASSERT_PTR_NOT_NULL(buffer);

    // after realloc, counts should be 1
    ret = mmem_dump(MMEM_DUMP_CMD_COUNTS, (void *)&counts, sizeof(counts));
    CU_ASSERT_EQUAL(ret, MMEM_DUMP_RET_OK);
    CU_ASSERT_EQUAL(counts, 1);

    // realloc buffer to 0 bytes(should be free)
    buffer = realloc(buffer, 0);
    CU_ASSERT_PTR_NULL(buffer);

    // after realloc, counts should be 0
    ret = mmem_dump(MMEM_DUMP_CMD_COUNTS, (void *)&counts, sizeof(counts));
    CU_ASSERT_EQUAL(ret, MMEM_DUMP_RET_OK);
    CU_ASSERT_EQUAL(counts, 0);
}

#define dump_test4_description "cmd: MMEM_DUMP_CMD_MMEM_INFO, get mmem_info"
void dump_test4(void)
{
    long ret = 0;
    mmem_info_t mmem_info = {0};
    char *buffer = NULL;

    ret = mmem_dump(MMEM_DUMP_CMD_MMEM_INFO, (void *)&mmem_info, sizeof(mmem_info));
    CU_ASSERT_EQUAL(ret, MMEM_DUMP_RET_OK);
    CU_ASSERT_EQUAL(mmem_info.counts, 0);
    CU_ASSERT_EQUAL(mmem_info.total_size, 0);
    CU_ASSERT_EQUAL(mmem_info.active_size, 0);

    // malloc a 2048 bytes buffer(2KB)
    buffer = malloc(2048);
    CU_ASSERT_PTR_NOT_NULL(buffer);

    ret = mmem_dump(MMEM_DUMP_CMD_MMEM_INFO, (void *)&mmem_info, sizeof(mmem_info));
    CU_ASSERT_EQUAL(ret, MMEM_DUMP_RET_OK);
    CU_ASSERT_EQUAL(mmem_info.counts, 1);
    CU_ASSERT_EQUAL(mmem_info.active_size, 2048);
    CU_ASSERT_EQUAL(mmem_info.max_active_size, 2048);

    free(buffer);

    ret = mmem_dump(MMEM_DUMP_CMD_MMEM_INFO, (void *)&mmem_info, sizeof(mmem_info));
    CU_ASSERT_EQUAL(ret, MMEM_DUMP_RET_OK);
    CU_ASSERT_EQUAL(mmem_info.counts, 0);
    CU_ASSERT_EQUAL(mmem_info.active_size, 0);
    CU_ASSERT_EQUAL(mmem_info.max_active_size, 2048);
}

#define dump_test5_description "cmd: MMEM_DUMP_CMD_MMEM_BLOCK_INFO, get mmem_block_info"
void dump_test5(void)
{
    long ret = 0;
    mmem_block_info_t mmem_info[5] = {0};
    char *buffer = NULL;

    // dump block info
    ret = mmem_dump(MMEM_DUMP_CMD_MMEM_BLOCK_INFO, (void *)mmem_info, sizeof(mmem_info));
    CU_ASSERT_EQUAL(ret, 0);

    // malloc a 2048 bytes buffer(2KB)
    buffer = malloc(2048);
    CU_ASSERT_PTR_NOT_NULL(buffer);

    // dump block info
    ret = mmem_dump(MMEM_DUMP_CMD_MMEM_BLOCK_INFO, (void *)mmem_info, sizeof(mmem_info));
    CU_ASSERT_EQUAL(ret, 1);
    CU_ASSERT_EQUAL(mmem_info[0].size, 2048);
    CU_ASSERT_PTR_NOT_NULL(mmem_info[0].file);
    CU_ASSERT_NOT_EQUAL(mmem_info[0].line, 0);

    free(buffer);
}

void dump_test_add_test(void)
{
    CU_pSuite dump_suite = CU_add_suite("dump test", NULL, NULL);

    // arg test
    CU_add_test(dump_suite, dump_test1_description, dump_test1);

    // dump counts
    CU_add_test(dump_suite, dump_test2_description, dump_test2);
    CU_add_test(dump_suite, dump_test3_description, dump_test3);

    // dump mem info
    CU_add_test(dump_suite, dump_test4_description, dump_test4);

    // dump block info
    CU_add_test(dump_suite, dump_test5_description, dump_test5);
}
