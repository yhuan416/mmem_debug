#include "tests.h"

#define free_all_test1_description "free_all, counts should be 0"
void free_all_test1(void)
{
    // malloc 10 bytes
    char *buffer = malloc(10);
    CU_ASSERT_PTR_NOT_NULL(buffer);

    // calloc 10 bytes
    char *buffer2 = calloc(1, 10);
    CU_ASSERT_PTR_NOT_NULL(buffer2);

    // get counts
    unsigned long counts = mmem_get_counts();
    CU_ASSERT_EQUAL(counts, 2);

    // free all
    mmem_free_all();

    // get counts
    counts = mmem_get_counts();
    CU_ASSERT_EQUAL(counts, 0);
}

void free_all_test_add_test(void)
{
    CU_add_test(g_suite, free_all_test1_description, free_all_test1);
}
