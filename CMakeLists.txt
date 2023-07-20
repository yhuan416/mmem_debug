cmake_minimum_required(VERSION 3.5)
project(mmem_debug)

option(MMEM_DEBUG_BUILD_TEST "Build mmem_debug test" ON)

if (DEFINED MMEM_DEBUG_BUILD_TEST)
    message(STATUS "mmem_debug: Build test: ${MMEM_DEBUG_BUILD_TEST}")
endif ()

if (MMEM_DEBUG_BUILD_TEST)
    enable_testing()
endif ()

file(GLOB MMEM_DEBUG_SRC src/*.c)

add_library(mmem_debug STATIC ${MMEM_DEBUG_SRC})
target_include_directories(mmem_debug PUBLIC include)

# mlist
add_subdirectory(lib/mlist-0.0.1)
target_link_libraries(mmem_debug PRIVATE mlist)

# mmem_debug_adapter
add_subdirectory(adapter)
target_link_libraries(mmem_debug PRIVATE mmem_debug_adapter)

# test build
if (MMEM_DEBUG_BUILD_TEST)
    # disable tests in CUnit
    set(CUNIT_DISABLE_TESTS TRUE)
    add_subdirectory(third_party/cunit-3.2.7/CUnit)

    # add test
    add_subdirectory(test)
endif ()
