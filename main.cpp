#include <iostream>
#include <string>

#include "huffmanAlgorithm.hpp"

int main()
{
  while (true) {
    std::cout << "Please, choose operating mode:\n0 - Exit\n1 - Compressing of file\n2 - Decompressing of file\n3 - Demonstrative mode\n";
    std::size_t mode = 0;
    std::cin >> mode;
    std::string path1;
    std::string path2;
    try {
      switch (mode) {
        case 0:
          std::cout << "Program execution ended\n";
          return 0;
        case 1:
          std::cout << "Enter path to file you want to compress:\n";
          std::cin.ignore();
          std::getline(std::cin, path1);
          autoCompress(path1);
          std::cout << '\n';
          std::cout << "File was compressed successfully!\n \n";
          break;

        case 2:
          std::cout << "Enter path to file you want to decompress:\n";
          std::cin.ignore();
          std::getline(std::cin, path1);
          std::cout << "Enter path to codes table:\n";
          std::getline(std::cin, path2);

          autoDecompress(path2, path1);
          std::cout << "File was decompressed successfully!\n \n";
          break;

        case 3:
          std::cout << "Enter path to file you want to use for demonstrative mode:\n";
          std::cin.ignore();
          std::getline(std::cin, path1);

          demonstrateHuffmanAlgorithm(path1);
          std::cout << '\n';
          std::cout << "Demonstration ended successfully!\n \n";
          break;

        default:
          std::cout << "Invalid mode input!\n \n";
          continue;
      }
    } catch (const std::exception &exception) {
      std::cout << exception.what() << "\n \n";
      continue;
    }
  }
}
