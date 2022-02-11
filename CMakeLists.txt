cmake_minimum_required(VERSION 3.21)
project(HuffmanArchiver)

set(CMAKE_CXX_STANDARD 20)

add_executable(HuffmanArchiver main.cpp huffmanBinaryTree.cpp huffmanBinaryTree.hpp huffmanAlgorithm.cpp huffmanAlgorithm.hpp)
