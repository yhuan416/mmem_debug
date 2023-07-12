cmake_minimum_required(VERSION 3.5)

add_executable(mmem_debug_test main.c)
target_link_libraries(mmem_debug_test mmem_debug)

add_test(NAME mmem_debug_test COMMAND mmem_debug_test)
