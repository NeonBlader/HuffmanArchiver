#include "huffmanBinaryTree.hpp"

HuffmanBinaryTree::HuffmanTreeNode::HuffmanTreeNode(int symbol, int frequency, HuffmanBinaryTree::HuffmanTreeNode *left, HuffmanBinaryTree::HuffmanTreeNode *right):
        key_({static_cast<std::uint8_t>(symbol), frequency}),
        left_(left),
        right_(right)
{}

HuffmanBinaryTree::HuffmanTreeNode::HuffmanTreeNode(std::uint8_t symbol, int frequency, HuffmanBinaryTree::HuffmanTreeNode *left, HuffmanBinaryTree::HuffmanTreeNode *right):
        key_({symbol, frequency}),
        left_(left),
        right_(right)
{}

bool HuffmanBinaryTree::operator>(const HuffmanBinaryTree &other) const
{
  return root_->key_.second > other.root_->key_.second;
}

HuffmanBinaryTree::HuffmanTreeNode *HuffmanBinaryTree::move()
{
  auto *tmp = root_;
  root_ = nullptr;
  return tmp;
}

HuffmanBinaryTree::~HuffmanBinaryTree()
{
  deleteSubtree(root_);
}

void HuffmanBinaryTree::deleteSubtree(HuffmanBinaryTree::HuffmanTreeNode *node)
{
  if (node == nullptr) {
    return;
  }
  deleteSubtree(node->left_);
  deleteSubtree(node->right_);
  delete node;
}

HuffmanBinaryTree::HuffmanBinaryTree(const HuffmanBinaryTree &other): HuffmanBinaryTree(nullptr)
{
  if (other.root_ == nullptr) {
    return;
  }
  root_ = copySubtree(other.root_);
}

HuffmanBinaryTree::HuffmanTreeNode *HuffmanBinaryTree::copySubtree(HuffmanBinaryTree::HuffmanTreeNode *otherNode) const
{
  if (otherNode == nullptr) {
    return nullptr;
  }
  auto *node = new HuffmanTreeNode(otherNode->key_.first, otherNode->key_.second);
  node->left_ = copySubtree(otherNode->left_);
  node->right_ = copySubtree(otherNode->right_);
  return node;
}

HuffmanBinaryTree &HuffmanBinaryTree::operator=(const HuffmanBinaryTree &other)
{
  HuffmanBinaryTree tmp(other);
  swap(tmp);
  return *this;
}

HuffmanBinaryTree::HuffmanTreeNode *HuffmanBinaryTree::getRoot() const
{
  return root_;
}

void HuffmanBinaryTree::swap(HuffmanBinaryTree &other)
{
  std::swap(root_, other.root_);
}

HuffmanBinaryTree &HuffmanBinaryTree::operator=(HuffmanBinaryTree &&other) noexcept
{
  swap(other);
  return *this;
}

HuffmanBinaryTree::HuffmanBinaryTree(HuffmanBinaryTree &&other) noexcept
{
  root_ = other.root_;
  other.root_ = nullptr;
}
