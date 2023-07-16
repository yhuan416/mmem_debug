#include "tests.h"

#define free_all_test1_description "free_all, counts should be 0"
void free_all_test1(void)
{
    char *buffer;
    char *buffer2;
    unsigned long counts;

    // malloc 10 bytes
    buffer = malloc(10);
    CU_ASSERT_PTR_NOT_NULL(buffer);

    // calloc 10 bytes
    buffer2 = calloc(1, 10);
    CU_ASSERT_PTR_NOT_NULL(buffer2);

    // get counts
    mmem_dump(MMEM_DUMP_CMD_COUNTS, (void *)&counts, sizeof(counts));
    CU_ASSERT_EQUAL(counts, 2);

    // free all
    mmem_free_all();

    // get counts
    mmem_dump(MMEM_DUMP_CMD_COUNTS, (void *)&counts, sizeof(counts));
    CU_ASSERT_EQUAL(counts, 0);
}

void free_all_test_add_test(void)
{
    CU_pSuite free_all_suite = CU_add_suite("free_all test", NULL, NULL);

    CU_add_test(free_all_suite, free_all_test1_description, free_all_test1);
}
