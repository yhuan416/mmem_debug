cmake_minimum_required(VERSION 3.5)

set(MMEM_DEBUG_TEST_NAME mmem_debug_test)

# disable tests in CUnit
set(CUNIT_DISABLE_TESTS TRUE)
add_subdirectory(cunit-3.2.7/CUnit)

set(tests 
    tests/malloc_test.c
    tests/calloc_test.c
    tests/realloc_test.c
    tests/free_test.c
    tests/free_all_test.c
)

set(TEST_FILES main.c ${tests})

add_executable(${MMEM_DEBUG_TEST_NAME} ${TEST_FILES})
include_directories(tests)

# mmem_debug
target_compile_definitions(${MMEM_DEBUG_TEST_NAME} PRIVATE __mmem_debug_enbale=1)
target_link_libraries(${MMEM_DEBUG_TEST_NAME} mmem_debug)

# CUnit
target_link_libraries(${MMEM_DEBUG_TEST_NAME} cunit)

add_test(NAME ${MMEM_DEBUG_TEST_NAME} COMMAND ${MMEM_DEBUG_TEST_NAME})
