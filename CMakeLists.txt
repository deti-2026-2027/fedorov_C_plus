cmake_minimum_required(VERSION 4.3)
project(oop__)

set(CMAKE_CXX_STANDARD 20)

add_executable(oop main.cpp
        word_stat.h
        word_stat.h
        word_stat.cpp
        text_analyzer.h
        text_analyzer.cpp
        csv_writer.cpp
        csv_writer.h
        error_handler.cpp
        error_handler.h)
