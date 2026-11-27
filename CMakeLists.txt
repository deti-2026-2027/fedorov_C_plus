cmake_minimum_required(VERSION 3.20)

project(lab1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Статическая библиотека с реализациями классов
add_library(cache_lib STATIC
        src/LRUCache.cpp
        src/LFUCache.cpp
        src/FibonacciCalculator.cpp
)

# Подключение заголовочных файлов
target_include_directories(cache_lib PUBLIC include)

# Основная программа
add_executable(lab1 src/main.cpp)

target_link_libraries(lab1 PRIVATE cache_lib)

# Загрузка Google Test
include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Настройка тестирования
enable_testing()

add_executable(cache_tests tests/tests.cpp)

target_link_libraries(cache_tests PRIVATE
        cache_lib
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(cache_tests)