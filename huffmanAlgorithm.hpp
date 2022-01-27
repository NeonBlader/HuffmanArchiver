#ifndef HUFFMAN_HUFFMANALGORITHM_HPP
#define HUFFMAN_HUFFMANALGORITHM_HPP

#include <iosfwd>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <cstdint>

#include "huffmanBinaryTree.hpp"

void demonstrateHuffmanAlgorithm(const std::string &sourceFilePath, const std::string &codesTablePath = "codes_table.txt",
    const std::string &codedFilePath = "coded_file.txt", const std::string &decodedFilePath = "decoded_file.txt",
    const std::string &compressedFilePath = "compressed_file.txt",
    const std::string &decompressedFilePath = "decompressed_file.txt");

void autoCompress(const std::string &sourceFilePath, const std::string &codesTablePath = "codes_table.txt",
    const std::string &compressedFilePath = "compressed_file.txt");

void autoDecompress(const std::string &tableFilePath, const std::string &compressedFilePath,
    const std::string &decompressedFilePath = "decompressed_file.txt");

#endif //HUFFMAN_HUFFMANALGORITHM_HPP
