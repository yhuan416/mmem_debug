#include "tests.h"

#define malloc_test1_description "malloc(1), should not return NULL"
void malloc_test1(void) {
    char *buffer = malloc(1);
    CU_ASSERT_PTR_NOT_NULL(buffer);
    free(buffer);
}

#define malloc_test2_description "malloc(0), should return NULL"
void malloc_test2(void) {
    char *buffer = malloc(0);
    CU_ASSERT_PTR_NULL(buffer);
    free(buffer);
}

void malloc_test_add_test(void)
{
    CU_add_test(g_suite, malloc_test1_description, malloc_test1);
    CU_add_test(g_suite, malloc_test2_description, malloc_test2);
}
