cmake_minimum_required(VERSION 3.24)
project(Lab10)

set(CMAKE_CXX_STANDARD 17)

add_executable(Lab10 main.cpp exp.h exp.cpp visitor.h visitor.cpp scanner.cpp scanner.h parser.cpp parser.h token.cpp token.h)
