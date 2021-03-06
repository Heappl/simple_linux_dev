cmake_minimum_required (VERSION 2.8)
project (median_dev_test)

set(dir ${PROJECT_SOURCE_DIR}/build/test)
set(EXECUTABLE_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)
set(LIBRARY_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)
set(CMAKE_CXX_FLAGS "--std=c++11 -g")
set(CMAKE_C_FLAGS "-g")

include_directories(${PROJECT_SOURCE_DIR}/googletest-release-1.8.0/gtest/include)
include_directories(${PROJECT_SOURCE_DIR}/googletest-release-1.8.0/googlemock/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

add_subdirectory (googletest-release-1.8.0)

add_executable(
    ${PROJECT_NAME}
    src/fixed_size_allocator.c
    src/allocator.c
    src/array.c
    src/heap.c
    src/median_container.c
    src/bigint.c
    src/median_dev.c
    test/linux_wrapper.cpp
    test/array_test.cpp
    test/heap_test.cpp
    test/allocator_test.cpp
    test/median_container_test.cpp
    test/bigint_test.cpp
    test/median_dev_test.cpp
)
target_link_libraries(${PROJECT_NAME} gtest gtest_main)
