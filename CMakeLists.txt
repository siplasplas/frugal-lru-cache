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
        IResizer.h
        HashMap.hpp gtest_hmap.cpp
        BaseHashMap.hpp BitScanner.cpp
        trees/IBinarySearchTree.h trees/BinarySearchTree.h
        trees/BaseBinaryTree.h trees/IBinaryTree.h
        trees/gtest_avl.cpp
        trees/AvlTree.h SlotTree.hpp AVLSlotTree.hpp PlanarCache.hpp)

target_link_libraries(${PROJECT_NAME} gtest gtest_main)

add_executable(test test.cpp lrucache.hpp
        MemInfo.cpp MemInfo.h
        BitScanner.cpp
        murmur.cpp SlotBits.hpp AVLSlotTree.hpp PlanarCache.hpp)
