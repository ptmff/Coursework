cmake_minimum_required(VERSION 3.27)
project(Coursework)

set(CMAKE_CXX_STANDARD 23)

add_executable(Coursework main.cpp
        termcolor/termcolor.hpp
        classes.hpp)
