cmake_minimum_required(VERSION 3.13)
project(gtest_lru)

add_definitions("-Werror=return-type")

add_executable(${PROJECT_NAME} lrucache.hpp gtest_lru.cpp
        List.hpp gtest_list.cpp
        LRU.hpp gtest_lru1.cpp
        HashMap.hpp gtest_map.cpp
        murmur.cpp)

target_link_libraries(${PROJECT_NAME} gtest gtest_main)

add_executable(test test.cpp lrucache.hpp
        MemInfo.cpp MemInfo.h
        List.hpp LRU.hpp HashMap.hpp)
