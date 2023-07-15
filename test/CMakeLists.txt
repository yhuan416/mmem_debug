cmake_minimum_required(VERSION 3.5)

set(MMEM_DEBUG_TEST_NAME mmem_debug_test)

set(tests 
    tests/malloc_test.c
    tests/calloc_test.c
    tests/realloc_test.c
    tests/free_test.c
)

set(TEST_FILES main.c ${tests})

add_executable(${MMEM_DEBUG_TEST_NAME} ${TEST_FILES})

include_directories(CUnit/include tests)

# mmem_debug
target_link_libraries(${MMEM_DEBUG_TEST_NAME} mmem_debug)

# CUnit
target_link_directories(${MMEM_DEBUG_TEST_NAME} PUBLIC CUnit/lib)
target_link_libraries(${MMEM_DEBUG_TEST_NAME} cunit)

add_test(NAME ${MMEM_DEBUG_TEST_NAME} COMMAND ${MMEM_DEBUG_TEST_NAME})
