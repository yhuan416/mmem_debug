#include "tests.h"

CU_pSuite g_suite = NULL;

static void mmem_debug_add_test(void)
{
    malloc_test_add_test();
    calloc_test_add_test();
    realloc_test_add_test();
    free_test_add_test();
    free_all_test_add_test();
}

int main() {
    CU_initialize_registry();
    g_suite = CU_add_suite("test_suite", NULL, NULL);

    mmem_debug_add_test();

    CU_basic_set_mode(CU_BRM_VERBOSE);
    
    CU_basic_run_tests();
    
    CU_cleanup_registry();
    
    return 0;
}