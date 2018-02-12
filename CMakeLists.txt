cmake_minimum_required (VERSION 2.8)
project (media_dev_test)

set(dir ${PROJECT_SOURCE_DIR}/build/test)
set(EXECUTABLE_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)
set(LIBRARY_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)
set(CMAKE_CXX_FLAGS "--std=c++11 -O3")

include_directories(${PROJECT_SOURCE_DIR}/googletest-release-1.8.0/gtest/include)
include_directories(${PROJECT_SOURCE_DIR}/googletest-release-1.8.0/gmock/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

add_subdirectory (googletest-release-1.8.0)

add_executable(
    ${PROJECT_NAME}
    src/median_dev.c
    test/linux_wrapper.cpp
    test/bigint.cpp
)
target_link_libraries(${PROJECT_NAME} gtest gtest_main)
