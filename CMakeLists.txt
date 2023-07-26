cmake_minimum_required(VERSION 3.5)

project(allocator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(allocator main.cpp)

install(TARGETS allocator
    RUNTIME DESTINATION bin)


if (MSVC)                                                             
    target_compile_options(allocator PRIVATE
        /W4
    )
else ()
    target_compile_options(allocator PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

#set_target_properties(allocator PROPERTIES COMPILE_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
#set_target_properties(allocator PROPERTIES LINK_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
