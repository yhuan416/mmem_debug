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

static void free_test1(void)
{
    char *buffer = malloc(1024);
    CU_ASSERT_PTR_NOT_NULL(buffer);
    free(buffer);
    CU_PASS("free(buffer) success");
}

static void free_test2(void)
{
    free(NULL);
    CU_PASS("free(NULL) success");
}

CUNIT_CI_RUN(CU_MAIN_EXE_NAME,
             CUNIT_CI_TEST(free_test1),
             CUNIT_CI_TEST(free_test2)
);
