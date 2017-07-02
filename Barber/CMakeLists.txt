cmake_minimum_required(VERSION 3.7)
project(Barber)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c)
add_executable(Barber ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(Barber pthread )