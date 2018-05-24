#ifndef _MAIN_HPP_
#define _MAIN_HPP_

#include "hzip.hpp"
#include "hfile.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

std::string getCompressedFilePath(const std::string& path_to_file);
std::string getDecompressedFilePath(const std::string& path_to_file);
void help();

#endif