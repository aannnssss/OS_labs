cmake_minimum_required(VERSION 3.27)
include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

set(CMAKE_CXX_STANDARD 17)

#add LAB1

add_executable(lab1_test lab1_test.cpp ../lab1/src/parent.c ../lab1/include/parent.h
../lab1/src/utils.c ../lab1/include/utils.h)

target_link_libraries(lab1_test PRIVATE GTest::GTest)
target_include_directories(lab1_test PRIVATE ../lab1/include)

add_test(lab1_test lab1_test)

#add LAB2

add_executable(lab2_test lab2_test.cpp ../lab2/include/lab2.hpp ../lab2/src/lab2.cpp ../lab2/include/utils.hpp)

target_link_libraries(lab2_test PRIVATE GTest::GTest)
target_include_directories(lab2_test PRIVATE ../lab2/include)

add_test(lab2_test lab2_test)