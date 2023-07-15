#include "tests.h"

#define calloc_test1_description "calloc(1, 10), should not return NULL"
void calloc_test1(void) {
    char *buffer = calloc(1, 10);
    CU_ASSERT_PTR_NOT_NULL(buffer);
    free(buffer);
}

#define calloc_test2_description "calloc(0, 10), should return NULL"
void calloc_test2(void) {
    char *buffer = calloc(0, 10);
    CU_ASSERT_PTR_NULL(buffer);
    free(buffer);
}

#define calloc_test3_description "calloc(1, 0), should return NULL"
void calloc_test3(void) {
    char *buffer = calloc(1, 0);
    CU_ASSERT_PTR_NULL(buffer);
    free(buffer);
}

void calloc_test_add_test(void)
{
    CU_add_test(g_suite, calloc_test1_description, calloc_test1);
    CU_add_test(g_suite, calloc_test2_description, calloc_test2);
    CU_add_test(g_suite, calloc_test3_description, calloc_test3);
}
