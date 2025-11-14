project(tests CXX)

cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#enable_testing() # Enable CTest to run tests

find_package(GTest REQUIRED)
#add_compile_options(-Wno-error=suggest-attribute=format)
include_directories(${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS})
add_compile_definitions(DEBUG)
add_executable(tests main.cc mecacipher.cc)
target_compile_options(tests PRIVATE -Wall -Wextra -O2 -Wno-error)
target_link_libraries(tests ${GTEST_LIBRARIES} gmock gmock_main pthread)
