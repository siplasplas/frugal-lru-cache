cmake_minimum_required(VERSION 3.13)
project(gtest_cache)

add_definitions("-Werror=return-type")

add_executable(${PROJECT_NAME} lrucache.hpp gtest_lru.cpp
        murmur.cpp
        gtest_smap.cpp
        SlotBits.hpp gtest_bits.cpp
        FrugalResizer.hpp BitScanner.h
        gtest_fresizer.cpp gtest_sresizer.cpp
        ArMap.hpp gtest_armap.cpp SimpleResizer.hpp
        IResizer.h)

target_link_libraries(${PROJECT_NAME} gtest gtest_main)

add_executable(test test.cpp lrucache.hpp
        MemInfo.cpp MemInfo.h
        murmur.cpp SlotBits.hpp)
