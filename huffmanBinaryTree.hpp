#ifndef HUFFMAN_HUFFMANBINARYTREE_HPP
#define HUFFMAN_HUFFMANBINARYTREE_HPP

#include <utility>
#include <string>
#include <map>
#include <cstdint>
#include <vector>

class HuffmanBinaryTree {
public:
  HuffmanBinaryTree(const HuffmanBinaryTree &other);
  HuffmanBinaryTree(HuffmanBinaryTree &&other) noexcept;
  ~HuffmanBinaryTree();

  bool operator>(const HuffmanBinaryTree &other) const;
  HuffmanBinaryTree &operator=(const HuffmanBinaryTree &other);
  HuffmanBinaryTree &operator=(HuffmanBinaryTree &&other) noexcept;

private:
  struct HuffmanTreeNode {
    explicit HuffmanTreeNode(std::uint8_t symbol, int frequency = 0, HuffmanBinaryTree::HuffmanTreeNode *left = nullptr,
        HuffmanBinaryTree::HuffmanTreeNode *right = nullptr);
    std::pair<std::uint8_t, int> key_;
    HuffmanTreeNode *left_;
    HuffmanTreeNode *right_;
  };

  friend HuffmanBinaryTree createHuffmanBinaryTree(const std::vector<int> &frequencyVector);
  friend std::map<std::uint8_t, std::vector<bool>> codeSymbols(const HuffmanBinaryTree &tree,
      HuffmanBinaryTree::HuffmanTreeNode *node, std::vector<bool> &code,std::map<std::uint8_t, std::vector<bool>> &map);
  friend void decodeFile(const std::string &codedFilePath, const std::string &decodedFilePath, const HuffmanBinaryTree &tree);
  friend void decompressFile(const HuffmanBinaryTree &tree, const std::string &compressedFilePath,
      const std::string &decompressedFilePath, unsigned int countOfBytes);
  friend HuffmanBinaryTree convertFileToTree(const std::string &tableFilePath, unsigned int &countOfBytes);
  friend void demonstrateHuffmanAlgorithm(const std::string &sourceFilePath, const std::string &codesTablePath,
      const std::string &codedFilePath, const std::string &decodedFilePath,
      const std::string &compressedFilePath, const std::string &decompressedFilePath);
  friend void autoCompress(const std::string &sourceFilePath, const std::string &codesTablePath,
                           const std::string &compressedFilePath);

  explicit HuffmanBinaryTree(HuffmanBinaryTree::HuffmanTreeNode *node):
          root_(node)
  {}

  HuffmanTreeNode *move();
  HuffmanTreeNode *getRoot() const;
  void deleteSubtree(HuffmanTreeNode *node);
  HuffmanTreeNode *copySubtree(HuffmanTreeNode *otherNode) const;
  void swap(HuffmanBinaryTree &other);

  HuffmanTreeNode *root_;
};

#endif //HUFFMAN_HUFFMANBINARYTREE_HPP
