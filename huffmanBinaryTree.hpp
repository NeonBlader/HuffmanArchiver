#ifndef HUFFMAN_HUFFMANBINARYTREE_HPP
#define HUFFMAN_HUFFMANBINARYTREE_HPP

#include <utility>
#include <iosfwd>
#include <map>
#include <cstdint>

class HuffmanAlgorithm;

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
    HuffmanTreeNode(int symbol, int frequency, HuffmanBinaryTree::HuffmanTreeNode *left = nullptr, HuffmanBinaryTree::HuffmanTreeNode *right = nullptr);
    explicit HuffmanTreeNode(std::uint8_t symbol, int frequency = 0, HuffmanBinaryTree::HuffmanTreeNode *left = nullptr, HuffmanBinaryTree::HuffmanTreeNode *right = nullptr);
    std::pair<std::uint8_t, int> key_;
    HuffmanTreeNode *left_;
    HuffmanTreeNode *right_;
  };

  friend HuffmanAlgorithm;

  explicit HuffmanBinaryTree(HuffmanBinaryTree::HuffmanTreeNode *node):
          root_(node)
  {}

  HuffmanTreeNode *move();
  HuffmanTreeNode *getRoot();
  void deleteSubtree(HuffmanTreeNode *node);
  HuffmanTreeNode *copySubtree(HuffmanTreeNode *otherNode) const;
  void swap(HuffmanBinaryTree &other);

  HuffmanTreeNode *root_;
};

#endif //HUFFMAN_HUFFMANBINARYTREE_HPP
