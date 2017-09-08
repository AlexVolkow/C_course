//
// Created by Alexandr Volkov on 21.03.2017.
//

#include "utils.h"
#include <stdexcept>

std::string replaceAll(std::string src, std::string oldStr, std::string newStr) {
    int it = src.find(oldStr, 0);
    while (it != std::string::npos) {
        src.replace(it, oldStr.size(), newStr);
        it = src.find(oldStr, it);
    }
    return src;
}

int combination(int n, int k) {
    int dp[n + 1][n + 1];
    for (int i = 0; i <= n; ++i) {
        dp[i][0] = 1;
        dp[i][i] = 1;
        for (int j = 1; j < i; ++j) {
            dp[i][j] = dp[i - 1][j - 1] + dp[i - 1][j];
        }
    }
    return dp[n][k];
}

std::vector<std::vector<char>> toScale(std::vector<std::vector<char>> v, int scale) {
    if (scale < 1) {
        throw std::logic_error("Scale < 1");
    }
    std::vector<std::vector<char>> res(v.size() * scale);
    for (int i = 0; i < res.size(); i++) {
        res[i] = std::vector<char>(v[0].size() * scale);
    }
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[0].size(); j++) {
            for (int t = i * scale; t < ((i + 1) * scale); t++) {
                for (int k = j * scale; k < ((j + 1) * scale); k++) {
                    res[t][k] = v[i][j];
                }
            }
        }
    }
    return res;
}

std::pair<std::string, std::string> parseCombination(std::string s) {
    int i = 2;
    int j = 2;
    int open = 1;
    std::string first;
    std::string second;
    while (i < s.length()) {
        if (s[i] == '(') {
            open++;
        }
        if (s[i] == ')') {
            open--;
        }
        if (s[i] == ',' && open == 1) {
            first = s.substr(j, i - j);
            j = i + 1;
        }
        if (open == 0) {
            second = s.substr(j, i - j);
            break;
        }
        i++;
    }
    return std::make_pair(first, second);
}
