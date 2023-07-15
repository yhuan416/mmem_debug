cmake_minimum_required(VERSION 3.5)
project(mmem_debug)

option(MMEM_DEBUG_BUILD_TEST "Build mmem_debug test" ON)

if (DEFINED MMEM_DEBUG_BUILD_TEST)
    message(STATUS "mmem_debug: Build test: ${MMEM_DEBUG_BUILD_TEST}")
endif ()

if (MMEM_DEBUG_BUILD_TEST)
    enable_testing()
endif ()

set(MMEM_DEBUG_SRC
    src/mmem_debug.c
    src/mmem_debug_adapter.c
)

add_library(mmem_debug STATIC ${MMEM_DEBUG_SRC})
target_include_directories(mmem_debug PUBLIC include)

if (MMEM_DEBUG_BUILD_TEST)
    add_subdirectory(test)
endif ()
