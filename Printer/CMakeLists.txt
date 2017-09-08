cmake_minimum_required(VERSION 3.6)
project(Printer)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp printer.h printer.cpp expression.h expression.cpp utils.h utils.cpp biginteger.h biginteger.cpp)
add_executable(Printer ${SOURCE_FILES})