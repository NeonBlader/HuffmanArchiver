#include "huffmanAlgorithm.hpp"

#include <iostream>
#include <queue>
#include <fstream>
#include <stdexcept>

const std::size_t BITS_COUNT = 8;

void HuffmanAlgorithm::getFrequencyTable()
{
  std::ifstream in(sourceFilePath_, std::ios::binary);
  if (!in.is_open()) {
    throw std::ios::failure("Can't open source file!");
  }
  frequencyVector_.resize(256);
  for (auto i: frequencyVector_) {
    i = 0;
  }
  while (true) {
    std::uint8_t symbol = in.get();
    if (in.eof()) {
      break;
    }
    frequencyVector_[symbol]++;
    countOfBytes_++;
  }
}

void HuffmanAlgorithm::demonstrateHuffmanAlgorithm()
{
  if (sourceFilePath_.empty()) {
    throw std::logic_error("You can't demonstrate Huffman algorithm without source file path");
  }

  std::vector<bool> code;

  getFrequencyTable();
  createHuffmanBinaryTree();
  codeSymbols(tree_.getRoot(), code);

  createCodesTable();

  createCodedFile();
  decodeFile();

  compressFile();
  decompressFile();
}

void HuffmanAlgorithm::createHuffmanBinaryTree()
{
  if (frequencyVector_.empty()) {
    throw std::logic_error("You can't create huffman binary tree without frequency vector!");
  }
  for (int i = 0; i < frequencyVector_.size(); ++i) {
    if (frequencyVector_[i] != 0) {
      uniqueBytes_++;
      auto *node = new HuffmanBinaryTree::HuffmanTreeNode(i, frequencyVector_[i]);
      queue_.push(HuffmanBinaryTree(node));
    }
  }
  while (queue_.size() != 1) {
    HuffmanBinaryTree firstTree = queue_.top();
    queue_.pop();
    HuffmanBinaryTree secondTree = queue_.top();
    queue_.pop();
    auto *node = new HuffmanBinaryTree::HuffmanTreeNode('\0', firstTree.getRoot()->key_.second + secondTree.getRoot()->key_.second, firstTree.move(), secondTree.move());
    queue_.push(HuffmanBinaryTree(node));
  }
  tree_ = queue_.top();
}

void HuffmanAlgorithm::codeSymbols(HuffmanBinaryTree::HuffmanTreeNode *node, std::vector<bool> &code)
{
  if (tree_.getRoot() == nullptr) {
    throw std::logic_error("You can't create code map without Huffman's tree");
  }
  if (node->key_.first != '\0') {
    codeMap_.emplace(node->key_.first, code);
  }
  if (node->left_ != nullptr) {
    std::vector<bool> tmpCode(code);
    code.push_back(false);
    codeSymbols(node->left_, code);
    code = tmpCode;
  }
  if (node->right_ != nullptr) {
    std::vector<bool> tmpCode(code);
    code.push_back(true);
    codeSymbols(node->right_, code);
    code = tmpCode;
  }
}

void HuffmanAlgorithm::createCodedFile()
{
  if (codeMap_.empty()) {
    throw std::logic_error("You can't create coded file without code map");
  }
  std::ifstream in(sourceFilePath_, std::ios::binary);
  if (!in.is_open()) {
    throw std::ios_base::failure("Can't open source file");
  }
  std::ofstream out(codedFilePath_);
  if (!out.is_open()) {
    throw std::ios_base::failure("Can't open file for writing coded information");
  }
  while (true) {
    std::uint8_t symbol = in.get();
    if (in.eof()) {
      break;
    }
    std::vector<bool> temp = codeMap_.find(symbol)->second;
    for (auto i: temp) {
      (i) ? out << '1' : out << '0';
    }
  }
}

void HuffmanAlgorithm::decodeFile()
{
  if (tree_.getRoot() == nullptr) {
    throw std::logic_error("You can't decode file without Huffman tree");
  }
  std::ifstream in(codedFilePath_);
  if (!in.is_open()) {
    throw std::ios_base::failure("Can't open coded file!");
  }
  std::ofstream out(decodedFilePath_);
  if (!out.is_open()) {
    throw std::ios_base::failure("Can't open decoded file!");
  }
  while (!in.eof()) {
    HuffmanBinaryTree::HuffmanTreeNode *node = tree_.getRoot();
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

void HuffmanAlgorithm::compressFile()
{
  if (codeMap_.empty()) {
    throw std::logic_error("You can't compress file without code map");
  }
  std::ifstream in(sourceFilePath_, std::ios::binary);
  if (!in.is_open()) {
    throw std::ios::failure("Cant' open file for reading!");
  }
  std::ofstream out(compressedFilePath_, std::ios::binary);
  if (!out.is_open()) {
    throw std::ios::failure("Cant' open file for writing!");
  }
  int count = 0;
  std::uint8_t buf = 0;
  std::uint8_t symbol;
  symbol = in.get();
  while (!in.eof()) {
    std::vector<bool> code = codeMap_[symbol];
    for (auto &&i: code) {
      buf = buf | i << ((BITS_COUNT - 1) - count);
      count++;
      if (count == BITS_COUNT) {
        count = 0;
        out << buf;
        buf = 0;
        countOfBytesAfterCompressing_++;
      }
    }
    symbol = in.get();
    if ((count != 0) && (in.eof())) {
      out << buf;
      countOfBytesAfterCompressing_++;
    }
  }
}

void HuffmanAlgorithm::decompressFile()
{
  if (tree_.getRoot() == nullptr) {
    throw std::logic_error("You can't decompress file without Huffman tree");
  }
  std::ifstream in(compressedFilePath_, std::ios::binary);
  if (!in.is_open()) {
    throw std::ios_base::failure("Can't open compressed file");
  }
  std::ofstream out(decompressedFilePath_);
  if (!in.is_open()) {
    throw std::ios_base::failure("Can't open file for decompressing");
  }
  HuffmanBinaryTree::HuffmanTreeNode *node = tree_.getRoot();
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
      if (outedCount == countOfBytes_) {
        break;
      }
      node = tree_.getRoot();
    }
    count++;
    if (count == BITS_COUNT) {
      count = 0;
      byte = in.get();
    }
  }
}

HuffmanAlgorithm::HuffmanAlgorithm():
        codedFilePath_("Coded.txt"),
        decodedFilePath_("Decoded.txt"),
        compressedFilePath_("Compressed.bin"),
        decompressedFilePath_("Decompressed.txt"),
        tableFilePath_("Table.txt"),
        tree_(nullptr),
        countOfBytes_(0),
        countOfBytesAfterCompressing_(0),
        uniqueBytes_(0)
{}

void HuffmanAlgorithm::setCodedFilePath(const std::string &codedPath)
{
  codedFilePath_ = codedPath;
}

void HuffmanAlgorithm::setDecodedFilePath(const std::string &decodedPath)
{
  decodedFilePath_ = decodedPath;
}

void HuffmanAlgorithm::setCompressedFilePath(const std::string &compressedPath)
{
  compressedFilePath_ = compressedPath;
}

void HuffmanAlgorithm::setDecompressedFilePath(const std::string &decompressedPath)
{
  decompressedFilePath_ = decompressedPath;
}

void HuffmanAlgorithm::setCodesTableFilePath(const std::string &codesTablePath)
{
  tableFilePath_ = codesTablePath;
}

void HuffmanAlgorithm::createCodesTable()
{
  std::ofstream out(tableFilePath_);
  std::cout << tableFilePath_ << '\n';
  if (!out.is_open()) {
    throw std::ios_base::failure("Can't open file for writing codes table");
  }
  out << countOfBytes_ << '\n';
  for (auto & iter : codeMap_) {
    out << iter.first << ' ';
    for (auto i: iter.second) {
      (i) ? out << '1' : out << '0';
    }
    out << '\n';
  }
}

void HuffmanAlgorithm::setHuffmanTree(const HuffmanBinaryTree &tree)
{
  tree_ = tree;
}

void HuffmanAlgorithm::convertFileToTree()
{
  std::ifstream in(tableFilePath_);
  if (!in.is_open()) {
    throw std::ios_base::failure("Can't open file with coded tree");
  }
  auto *startNode = new HuffmanBinaryTree::HuffmanTreeNode('\0');
  HuffmanBinaryTree tree(startNode);
  in >> countOfBytes_;
  while (!in.eof()) {
    std::cout << tree.getRoot()->key_.first;
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
  tree_ = tree;
}

void HuffmanAlgorithm::setSourceFilePath(const std::string &sourcePath)
{
  sourceFilePath_ = sourcePath;
}

void HuffmanAlgorithm::autoCompress()
{
  std::vector<bool> code;
  getFrequencyTable();
  createHuffmanBinaryTree();
  codeSymbols(tree_.getRoot(), code);
  createCodesTable();
  compressFile();
}

void HuffmanAlgorithm::autoDecompress()
{
  convertFileToTree();
  decompressFile();
}

void HuffmanAlgorithm::printStats(std::ostream &out)
{
  out << "Count of unique bytes in source file: " << uniqueBytes_ << '\n';
  out << "Count of bytes before compressing: " << countOfBytes_ << '\n';
  out << "Count of bytes after compressing: " << countOfBytesAfterCompressing_ << '\n';
  out << "Ideally, it would be saved " << 100 - (static_cast<double>(countOfBytesAfterCompressing_) / countOfBytes_) * 100 << "% of memory";
}
