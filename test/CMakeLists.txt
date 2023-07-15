cmake_minimum_required(VERSION 3.5)

set(MMEM_DEBUG_TEST_NAME mmem_debug_test)
set(TEST_FILES main.c)

add_executable(${MMEM_DEBUG_TEST_NAME} ${TEST_FILES})

# mmem_debug
target_link_libraries(${MMEM_DEBUG_TEST_NAME} mmem_debug)

# CUnit
target_link_directories(${MMEM_DEBUG_TEST_NAME} PUBLIC CUnit/lib)
target_link_libraries(${MMEM_DEBUG_TEST_NAME} cunit)
include_directories(CUnit/include)

add_test(NAME ${MMEM_DEBUG_TEST_NAME} COMMAND ${MMEM_DEBUG_TEST_NAME})
