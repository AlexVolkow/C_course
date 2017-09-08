//
// Created by Alexandr Volkov on 21.03.2017.
//

#ifndef PRINTER_UTILS_H
#define PRINTER_UTILS_H

#include <string>
#include <vector>

std::string replaceAll(std::string src, std::string oldStr, std::string newStr);

int combination(int n, int k);

std::pair<std::string, std::string> parseCombination(std::string s);

std::vector<std::vector<char>> toScale(std::vector<std::vector<char>> v, int scale);

#endif //PRINTER_UTILS_H
