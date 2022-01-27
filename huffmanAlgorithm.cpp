#include "huffmanAlgorithm.hpp"

#include <iostream>
#include <queue>
#include <fstream>
#include <stdexcept>

const std::size_t BITS_COUNT = 8;

std::vector<int> getFrequencyTable(const std::string &sourceFilePath, unsigned int &countOfBytes)
{
  std::ifstream in(sourceFilePath, std::ios::binary);
  if (!in.is_open()) {
    throw std::ios::failure("Can't open source file!");
  }
  std::vector<int> frequencyVector(256);
  while (true) {
    std::uint8_t symbol = in.get();
    if (in.eof()) {
      break;
    }
    if (symbol != '\r') {
      frequencyVector[symbol]++;
      countOfBytes++;
    }
  }
  return frequencyVector;
}

HuffmanBinaryTree createHuffmanBinaryTree(const std::vector<int> &frequencyVector)
{
  if (frequencyVector.empty()) {
    throw std::logic_error("You can't create huffman binary tree without frequency vector!");
  }
  unsigned int uniqueBytes = 0;
  std::priority_queue<HuffmanBinaryTree, std::vector<HuffmanBinaryTree>, std::greater<>> queue;
  for (int i = 0; i < frequencyVector.size(); ++i) {
    if (frequencyVector[i] != 0) {
      uniqueBytes++;
      auto *node = new HuffmanBinaryTree::HuffmanTreeNode(i, frequencyVector[i]);
      queue.push(HuffmanBinaryTree(node));
    }
  }
  while (queue.size() != 1) {
    HuffmanBinaryTree firstTree = queue.top();
    queue.pop();
    HuffmanBinaryTree secondTree = queue.top();
    queue.pop();
    auto *node = new HuffmanBinaryTree::HuffmanTreeNode('\0', firstTree.getRoot()->key_.second + secondTree.getRoot()->key_.second, firstTree.move(), secondTree.move());
    queue.push(HuffmanBinaryTree(node));
  }
  HuffmanBinaryTree tree = queue.top();
  return tree;
}

std::map<std::uint8_t, std::vector<bool>> codeSymbols(const HuffmanBinaryTree &tree, HuffmanBinaryTree::HuffmanTreeNode *node, std::vector<bool> &code,
                 std::map<std::uint8_t, std::vector<bool>> &codeMap)
{
  if (tree.getRoot() == nullptr) {
    throw std::logic_error("You can't create code map without Huffman's tree");
  }
  if (node->key_.first != '\0') {
    codeMap.emplace(node->key_.first, code);
  }
  if (node->left_ != nullptr) {
    std::vector<bool> tmpCode(code);
    code.push_back(false);
    codeSymbols(tree, node->left_, code, codeMap);
    code = tmpCode;
  }
  if (node->right_ != nullptr) {
    std::vector<bool> tmpCode(code);
    code.push_back(true);
    codeSymbols(tree, node->right_, code, codeMap);
    code = tmpCode;
  }
  return codeMap;
}

void createCodedFile(const std::string &sourceFilePath, const std::string &codedFilePath,
                     std::map<std::uint8_t, std::vector<bool>> &codeMap)
{
  if (codeMap.empty()) {
    throw std::logic_error("You can't create coded file without code map");
  }
  std::ifstream in(sourceFilePath, std::ios::binary);
  if (!in.is_open()) {
    throw std::ios_base::failure("Can't open source file");
  }
  std::ofstream out(codedFilePath);
  if (!out.is_open()) {
    throw std::ios_base::failure("Can't open file for writing coded information");
  }
  while (true) {
    std::uint8_t symbol = in.get();
    if (in.eof()) {
      break;
    }
    if (symbol != '\r') {
      std::vector<bool> temp = codeMap.find(symbol)->second;
      for (auto i: temp) {
        (i) ? out << '1' : out << '0';
      }
    }
  }
}

void decodeFile(const std::string &codedFilePath, const std::string &decodedFilePath, const HuffmanBinaryTree &tree)
{
  if (tree.getRoot() == nullptr) {
    throw std::logic_error("You can't decode file without Huffman tree");
  }
  std::ifstream in(codedFilePath);
  if (!in.is_open()) {
    throw std::ios_base::failure("Can't open coded file!");
  }
  std::ofstream out(decodedFilePath);
  if (!out.is_open()) {
    throw std::ios_base::failure("Can't open decoded file!");
  }
  while (!in.eof()) {
    HuffmanBinaryTree::HuffmanTreeNode *node = tree.getRoot();
    while (node->key_.first == '\0') {
      std::uint8_t symbol = in.get();
      if (in.eof()) {
        break;
      }
      if (symbol == '1') {
        node = node->right_;
      } else {
        node = node->left_;
      }
    }
    out << node->key_.first;
  }
}

void compressFile(const std::string &sourceFilePath, const std::string &compressedFilePath,
                  std::map<std::uint8_t, std::vector<bool>> &codeMap)
{
  if (codeMap.empty()) {
    throw std::logic_error("You can't compress file without code map");
  }
  std::ifstream in(sourceFilePath, std::ios::binary);
  if (!in.is_open()) {
    throw std::ios::failure("Cant' open file for reading!");
  }
  std::ofstream out(compressedFilePath, std::ios::binary);
  if (!out.is_open()) {
    throw std::ios::failure("Cant' open file for writing!");
  }
  int count = 0;
  std::uint8_t buf = 0;
  std::uint8_t symbol;
  symbol = in.get();
  while (!in.eof()) {
    std::vector<bool> code = codeMap[symbol];
    for (auto &&i: code) {
      buf = buf | i << ((BITS_COUNT - 1) - count);
      count++;
      if (count == BITS_COUNT) {
        count = 0;
        out << buf;
        buf = 0;
      }
    }
    symbol = in.get();
    if ((count != 0) && (in.eof())) {
      out << buf;
    }
  }
}

void decompressFile(const HuffmanBinaryTree &tree, const std::string &compressedFilePath, const std::string &decompressedFilePath,
  unsigned int countOfBytes)
{
  if (tree.getRoot() == nullptr) {
    throw std::logic_error("You can't decompress file without Huffman tree");
  }
  std::ifstream in(compressedFilePath, std::ios::binary);
  if (!in.is_open()) {
    throw std::ios_base::failure("Can't open compressed file");
  }
  std::ofstream out(decompressedFilePath);
  if (!in.is_open()) {
    throw std::ios_base::failure("Can't open file for decompressing");
  }
  HuffmanBinaryTree::HuffmanTreeNode *node = tree.getRoot();
  int count = 0;
  std::uint8_t byte;
  byte = in.get();
  int outedCount = 0;
  while (!in.eof()) {
    bool binary = byte & (true << ((BITS_COUNT - 1) - count));
    if (binary) {
      node = node->right_;
    } else {
      node = node->left_;
    }
    if (node->key_.first != '\0') {
      out << node->key_.first;
      outedCount++;
      if (outedCount == countOfBytes) {
        break;
      }
      node = tree.getRoot();
    }
    count++;
    if (count == BITS_COUNT) {
      count = 0;
      byte = in.get();
    }
  }
}

void createCodesTable(const std::map<std::uint8_t, std::vector<bool>> &codeMap, const std::string &tableFilePath,
    unsigned int countOfBytes)
{
  std::ofstream out(tableFilePath);
  if (!out.is_open()) {
    throw std::ios_base::failure("Can't open file for writing codes table");
  }
  out << countOfBytes << '\n';
  for (const auto & iter : codeMap) {
    out << iter.first << ' ';
    for (auto i: iter.second) {
      (i) ? out << '1' : out << '0';
    }
    out << '\n';
  }
}

HuffmanBinaryTree convertFileToTree(const std::string &tableFilePath, unsigned int &countOfBytes)
{
  std::ifstream in(tableFilePath);
  if (!in.is_open()) {
    throw std::ios_base::failure("Can't open file with coded tree");
  }
  auto *startNode = new HuffmanBinaryTree::HuffmanTreeNode('\0');
  HuffmanBinaryTree tree(startNode);
  in >> countOfBytes;
  while (!in.eof()) {
    std::uint8_t currentSymbol = in.get();
    in.get();
    std::string branchesCode;
    std::getline(in, branchesCode);
    auto current = tree.getRoot();
    for (std::uint8_t i : branchesCode) {
      if (i == '0') {
        if (current->left_ == nullptr) {
          current->left_ = new HuffmanBinaryTree::HuffmanTreeNode('\0');
        }
        current = current->left_;
      } else if (i == '1') {
        if (current->right_ == nullptr) {
          current->right_ = new HuffmanBinaryTree::HuffmanTreeNode('\0');
        }
        current = current->right_;
      }
    }
    if (in.eof()) {
      break;
    }
    current->key_.first = currentSymbol;
  }
  return tree;
}

void autoCompress(const std::string &sourceFilePath, const std::string &codesTablePath,
  const std::string &compressedFilePath)
{
  unsigned int countOfBytes;
  std::vector<int> frequencyTable = getFrequencyTable(sourceFilePath, countOfBytes);
  HuffmanBinaryTree tree = createHuffmanBinaryTree(frequencyTable);

  std::map<std::uint8_t, std::vector<bool>> codeMap;
  std::vector<bool> code;
  codeSymbols(tree, tree.getRoot(), code, codeMap);

  createCodesTable(codeMap, codesTablePath, countOfBytes);
  compressFile(sourceFilePath, compressedFilePath, codeMap);
}

void autoDecompress(const std::string &tableFilePath, const std::string &compressedFilePath, const std::string &decompressedFilePath)
{
  unsigned int countOfBytes = 0;
  HuffmanBinaryTree tree = convertFileToTree(tableFilePath, countOfBytes);
  decompressFile(tree, compressedFilePath, decompressedFilePath,countOfBytes);
}

void demonstrateHuffmanAlgorithm(const std::string &sourceFilePath, const std::string &codesTablePath,
                                 const std::string &codedFilePath, const std::string &decodedFilePath,
                                 const std::string &compressedFilePath, const std::string &decompressedFilePath)
{
  if (sourceFilePath.empty()) {
    throw std::logic_error("You can't demonstrate Huffman algorithm without source file path");
  }

  unsigned int countOfBytes;
  std::vector<int> frequencyVector = getFrequencyTable(sourceFilePath,  countOfBytes);
  HuffmanBinaryTree tree = createHuffmanBinaryTree(frequencyVector);

  std::map<std::uint8_t, std::vector<bool>> codeMap;
  std::vector<bool> code;
  codeSymbols(tree, tree.getRoot(), code, codeMap);

  createCodesTable(codeMap, codesTablePath, countOfBytes);
  createCodedFile(sourceFilePath, codedFilePath, codeMap);
  decodeFile(codedFilePath, decodedFilePath, tree);

  compressFile(sourceFilePath, compressedFilePath, codeMap);
  decompressFile(tree, compressedFilePath, decompressedFilePath, countOfBytes);
}
