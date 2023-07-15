#include "tests.h"

#define free_test1_description "free(buffer), should success"
void free_test1(void) {
    char *buffer = malloc(1);
    CU_ASSERT_PTR_NOT_NULL(buffer);
    free(buffer);
    CU_PASS("free(buffer) success");
}

#define free_test2_description "free(NULL), should success"
void free_test2(void)
{
    free(NULL);
    CU_PASS("free(NULL) success");
}

void free_test_add_test(void)
{
    CU_add_test(g_suite, free_test1_description, free_test1);
    CU_add_test(g_suite, free_test2_description, free_test2);
}
