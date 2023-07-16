#include "common.h"

/* run at the start of each suite */
CU_SUITE_SETUP() {
    return CUE_SUCCESS;
}

/* run at the end of the suite */
CU_SUITE_TEARDOWN() {
    return CUE_SUCCESS;
}

/* run at the start of each test */
CU_TEST_SETUP() {

}

/* run at the end of each test */
CU_TEST_TEARDOWN() {

}

static void realloc_test1(void)
{
    char *buffer = malloc(1024);
    CU_ASSERT_PTR_NOT_NULL(buffer);
    buffer = realloc(buffer, 2048);
    CU_ASSERT_PTR_NOT_NULL(buffer);
    free(buffer);
}

static void realloc_test2(void)
{
    char *buffer = realloc(NULL, 2048);
    CU_ASSERT_PTR_NOT_NULL(buffer);
    free(buffer);
}

static void realloc_test3(void)
{
    char *buffer = malloc(1024);
    CU_ASSERT_PTR_NOT_NULL(buffer);
    buffer = realloc(buffer, 0);
    CU_ASSERT_PTR_NULL(buffer);
    free(buffer);
}

static void realloc_test4(void)
{
    char *buffer = realloc(NULL, 0);
    CU_ASSERT_PTR_NULL(buffer);
    free(buffer);
}

CUNIT_CI_RUN(CU_MAIN_EXE_NAME,
             CUNIT_CI_TEST(realloc_test1),
             CUNIT_CI_TEST(realloc_test2),
             CUNIT_CI_TEST(realloc_test3),
             CUNIT_CI_TEST(realloc_test4)
);
