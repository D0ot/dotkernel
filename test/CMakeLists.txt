cmake_minimum_required(VERSION 3.3)

project(dotkernel_test)

add_executable(bitmap_test bitmap_test.cpp ../libs/log.c ../libs/kustd.c ../libs/utils.c)
target_compile_options(bitmap_test PUBLIC -I${PROJECT_PATH})