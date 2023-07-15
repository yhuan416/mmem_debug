#ifndef _TEST_H_
#define _TEST_H_

#include <CUnit/Basic.h>

#include "mmem_debug.h"

extern CU_pSuite g_suite;


void malloc_test_add_test(void);

void calloc_test_add_test(void);

void free_test_add_test(void);

void realloc_test_add_test(void);

void free_all_test_add_test(void);

#endif // !_TEST_H_
