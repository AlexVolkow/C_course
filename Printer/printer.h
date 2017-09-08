//
// Created by Alexandr Volkov on 06.03.2017.
//

#ifndef PRINTER_PRINTER_H
#define PRINTER_PRINTER_H

#include <map>
#include <vector>


class Printer {
private:
    static const int COUNT_LETTERS = 10;
    static const unsigned int SIZE_NUMBER = 5;
    static std::string let[COUNT_LETTERS];

    std::map<char, int *> alph;

    static const int BUFFER_HEIGHT = SIZE_NUMBER * 100 + 1;
    static const int BUFFER_WIDTH = 1000;
    char **buffer;
    unsigned int bufferSize = 0;
    int maxScale = 1;

    void putCharInBuffer(int x, int y, char ch, int scale);

    int putStringInBuffer(int x, int y, std::string s, int scale);

    int countNesting(std::string);

    void putSeparator(int x, int y);

    bool isSpaceLine(int x);

    void clearBuffer();

    std::vector<std::vector<char>> activeBuffer();

public:
    Printer();

    void putChar(char v);

    void putInt(int v);

    void print();

    void putString(std::string v);

    void saveToBMP(std::string file);

    void clear();
};

#endif //PRINTER_PRINTER_H
