# Huffman Archiver
This repository contains the source code of a program that implements the compression of files with text information using the Huffman algorithm. 

This program is designed to work with text files presented in UTF-8 encoding or another encoding that requires 8 bits to store one character.

After including the huffmanAlgorithm.hpp, three functions will become available to you:

*     autoCompress(const std::string &sourceFilePath, const std::string &codesTablePath,
      const std::string &compressedFilePath)

  This function is designed to compress a file located at the path passed
  in the *sourceFilePath* argument.  
  Argument *codesTablePath* is intended to specify the path where the file with information about how the characters were encoded will be saved. It is **necessary** for subsequent decompression of the file.  
  Argument *compressedFilePath* specifies the path where the resulting compressed file will be stored.  
  If the user does not set the last two parameters, the default values "coded_table.txt" and "compressed_file_txt" will be used. In this case, files with these names will be saved in the directory with the executable file.  

*     void autoDecompress(const std::string &tableFilePath, const std::string &compressedFilePath,
      const std::string &decompressedFilePath);  
  This function is designed to decompress a previously compressed file.  
  Argument *tableFilePath* must contain the path to the encoding table obtained after applying *autoCompress*.  
  Argument *compressedFilePath* must contain the path to the compressed file obtained after applying *autoCompress*.  
  Argument *tableFilePath* sets the path where the file will be located after decompression. It has default value "decompressed_file.txt".

*     void demonstrateHuffmanAlgorithm(const std::string &sourceFilePath, const std::string &codesTablePath,
      const std::string &codedFilePath, const std::string &decodedFilePath,
      const std::string &compressedFilePath,
      const std::string &decompressedFilePath);  
  This function sequentially compresses and decompresses a file located along the path from sourceFilePath.  
  In addition, the function creates: 
  * An encoded file that clearly shows which bits are characters replaces by  
  * Codetable decoded version of this file  
  
  Similarly with the previous two functions, if the user does not specify names for all required files, the default names will be used and the files will be located in the directory with the executable file.

The Huffman algorithm is designed in such a way that the fewer unique characters are present in the file, the more memory gain it can give.  
Below is the memory gain that this implementation of the algorithm allows to obtain for a 6 kilobyte file with a different number of unique characters.

![Memory benefit graphic](https://imgur.com/a/3txjf6T)

I also present a graph of the time spent on compressing a file consisting of 7 different characters, duplicated a different number of times.

![Time usage graphic](https://imgur.com/a/Ef3ZRqH)