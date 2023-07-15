#include "tests.h"

#define dump_test1_description "mmem_dump, arg test"
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

#define dump_test2_description "mmem_dump, cmd: MMEM_DUMP_CMD_COUNTS, get block counts"
void dump_test2(void)
{
    long ret = 0;
    char *buffer = NULL;
    unsigned long counts = 0;

    buffer = malloc(10);
    CU_ASSERT_PTR_NOT_NULL(buffer);

    ret = mmem_dump(MMEM_DUMP_CMD_COUNTS, (void *)&counts, sizeof(counts));
    CU_ASSERT_EQUAL(ret, MMEM_DUMP_RET_OK);
    CU_ASSERT_EQUAL(counts, 1);

    free(buffer);

    ret = mmem_dump(MMEM_DUMP_CMD_COUNTS, (void *)&counts, sizeof(counts));
    CU_ASSERT_EQUAL(ret, MMEM_DUMP_RET_OK);
    CU_ASSERT_EQUAL(counts, 0);
}

#define dump_test3_description "mmem_dump, cmd: MMEM_DUMP_CMD_MMEM_INFO, get mmem_info"
void dump_test3(void)
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

void dump_test_add_test(void)
{
    CU_pSuite dump_suite = CU_add_suite("dump test", NULL, NULL);

    CU_add_test(dump_suite, dump_test1_description, dump_test1);
    CU_add_test(dump_suite, dump_test2_description, dump_test2);
    CU_add_test(dump_suite, dump_test3_description, dump_test3);
}
