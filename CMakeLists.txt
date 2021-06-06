cmake_minimum_required(VERSION 3.17)
project(LAB_OGL_MOVING_MAN C)

set(CMAKE_C_STANDARD 99)

add_executable(LAB_OGL_MOVING_MAN main.c)
target_link_libraries(LAB_OGL_MOVING_MAN -lOpenGL32 )