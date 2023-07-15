#include <CUnit/Basic.h>

void test_add(void) {
    CU_ASSERT_EQUAL(2 + 2, 4);
}

void test_sub(void) {
    CU_ASSERT_EQUAL(2 - 2, 0);
}

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("test_suite", NULL, NULL);
    
    CU_add_test(suite, "test_add", test_add);
    CU_add_test(suite, "test_sub", test_sub);
    
    CU_basic_set_mode(CU_BRM_VERBOSE);
    
    CU_basic_run_tests();
    
    CU_cleanup_registry();
    
    return 0;
}