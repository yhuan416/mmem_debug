#include "tests.h"

#define realloc_test1_description "realloc(buffer, 20), should not return NULL"
void realloc_test1(void)
{
    char *buffer = malloc(10);
    CU_ASSERT_PTR_NOT_NULL(buffer);
    buffer = realloc(buffer, 20);
    CU_ASSERT_PTR_NOT_NULL(buffer);
    free(buffer);
}

#define realloc_test2_description "realloc(NULL, 20), should not return NULL"
void realloc_test2(void)
{
    char *buffer = realloc(NULL, 20);
    CU_ASSERT_PTR_NOT_NULL(buffer);
    free(buffer);
}

#define realloc_test3_description "realloc(buffer, 0), should return NULL"
void realloc_test3(void)
{
    char *buffer = malloc(10);
    CU_ASSERT_PTR_NOT_NULL(buffer);
    buffer = realloc(buffer, 0);
    CU_ASSERT_PTR_NULL(buffer);
    free(buffer);
}

#define realloc_test4_description "realloc(NULL, 0), should return NULL"
void realloc_test4(void)
{
    char *buffer = realloc(NULL, 0);
    CU_ASSERT_PTR_NULL(buffer);
    free(buffer);
}

void realloc_test_add_test(void)
{
    CU_add_test(g_suite, realloc_test1_description, realloc_test1);
    CU_add_test(g_suite, realloc_test2_description, realloc_test2);
    CU_add_test(g_suite, realloc_test3_description, realloc_test3);
    CU_add_test(g_suite, realloc_test4_description, realloc_test4);
}