cmake_minimum_required(VERSION 3.18)
project(mecacipher LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
if (MSVC)
    target_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror -O2)
endif()


add_subdirectory(tests)

