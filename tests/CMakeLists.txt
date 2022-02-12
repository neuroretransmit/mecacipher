project(tests NONE)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS})
add_compile_definitions(DEBUG)
add_executable(tests main.cc mecacipher.cc)
target_link_libraries(tests ${GTEST_LIBRARIES} gmock gmock_main pthread)
