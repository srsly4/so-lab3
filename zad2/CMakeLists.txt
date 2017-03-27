cmake_minimum_required(VERSION 3.0)
project(zad2)

set(CMAKE_C_FLAGS "--std=gnu99 -Wall ${CMAKE_C_FLAGS}")

add_executable(zad2 main.c)
add_executable(trash-prog trash_prog.c)
target_compile_options(trash-prog PRIVATE "-O0")