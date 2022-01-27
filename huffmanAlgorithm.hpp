#ifndef HUFFMAN_HUFFMANALGORITHM_HPP
#define HUFFMAN_HUFFMANALGORITHM_HPP

#include <iosfwd>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <cstdint>

#include "huffmanBinaryTree.hpp"

class HuffmanAlgorithm {
public:
  explicit HuffmanAlgorithm();

  void setSourceFilePath(const std::string &sourcePath);
  void setCodedFilePath(const std::string &codedPath);
  void setDecodedFilePath(const std::string &decodedPath);
  void setCompressedFilePath(const std::string &compressedPath);
  void setDecompressedFilePath(const std::string &decompressedPath);
  void setCodesTableFilePath(const std::string &codesTablePath);
  void setHuffmanTree(const HuffmanBinaryTree &tree);

  void demonstrateHuffmanAlgorithm();
  void autoCompress();
  void autoDecompress();

  void decodeFile(); //Remade
  void getFrequencyTable(); //Remade
  void createHuffmanBinaryTree(); //Remade
  void codeSymbols(HuffmanBinaryTree::HuffmanTreeNode *node, std::vector<bool> &code); //Remade
  void createCodedFile(); //Remade
  void createCodesTable(); //Remade
  void compressFile(); //Remade
  void decompressFile(); //Remade
  void printStats(std::ostream &out);

private:
  std::string sourceFilePath_;
  std::string codedFilePath_;
  std::string decodedFilePath_;
  std::string compressedFilePath_;
  std::string decompressedFilePath_;
  std::string tableFilePath_;

  std::vector<int> frequencyVector_;
  std::priority_queue<HuffmanBinaryTree, std::vector<HuffmanBinaryTree>, std::greater<>> queue_;
  std::map<std::uint8_t, std::vector<bool>> codeMap_;
  int countOfBytes_;
  int countOfBytesAfterCompressing_;
  int uniqueBytes_;
  HuffmanBinaryTree tree_;

  void convertFileToTree();
};

#endif //HUFFMAN_HUFFMANALGORITHM_HPP
