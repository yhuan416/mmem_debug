#include "tests.h"

static void mmem_debug_add_test(void)
{
    malloc_test_add_test();
    calloc_test_add_test();
    realloc_test_add_test();
    free_test_add_test();
    dump_test_add_test();
    free_all_test_add_test();
}

int main() {
    unsigned int result = 0;

    CU_initialize_registry();

    mmem_debug_add_test();

    CU_basic_set_mode(CU_BRM_VERBOSE);

    CU_basic_run_tests();

    result = CU_get_number_of_failures();

    CU_cleanup_registry();

    return result;
}