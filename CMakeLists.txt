cmake_minimum_required(VERSION 3.5)
project(mmem_debug)

enable_testing()

set(MMEM_DEBUG_ENABLE 1)

set(MMEM_DEBUG_SRC
    src/mmem_debug.c
    src/mmem_debug_adapter.c
)

add_library(mmem_debug STATIC ${MMEM_DEBUG_SRC})
target_include_directories(mmem_debug PUBLIC include)
target_compile_definitions(mmem_debug PUBLIC __mmem_debug_enbale=${MMEM_DEBUG_ENABLE})

add_subdirectory(test)
