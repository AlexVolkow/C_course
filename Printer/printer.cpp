//
// Created by Alexandr Volkov on 06.03.2017.
//

#include <iostream>
#include <fstream>
#include "printer.h"
#include "utils.h"

using namespace std;


string Printer::let[] = {"#####", "#   #", "#    ", "    #", "     ", "   # ", "  #  ", " #   ", " ### ", " # # "};

Printer::Printer() {
    buffer = new char*[BUFFER_HEIGHT];
    for (int i = 0;i < BUFFER_HEIGHT; i++) {
        buffer[i] = new char[BUFFER_WIDTH];
    }
    alph['0'] = new int[SIZE_NUMBER]{0, 1, 1, 1, 0};
    alph['1'] = new int[SIZE_NUMBER]{6, 6, 6, 6, 6};
    alph['2'] = new int[SIZE_NUMBER]{0, 3, 0, 2, 0};
    alph['3'] = new int[SIZE_NUMBER]{0, 3, 0, 3, 0};
    alph['4'] = new int[SIZE_NUMBER]{1, 1, 0, 3, 3};
    alph['5'] = new int[SIZE_NUMBER]{0, 2, 0, 3, 0};
    alph['6'] = new int[SIZE_NUMBER]{0, 2, 0, 1, 0};
    alph['7'] = new int[SIZE_NUMBER]{0, 3, 3, 3, 3};
    alph['8'] = new int[SIZE_NUMBER]{0, 1, 0, 1, 0};
    alph['9'] = new int[SIZE_NUMBER]{0, 1, 0, 3, 0};
    alph['('] = new int[SIZE_NUMBER]{3, 5, 6, 5, 3};
    alph[')'] = new int[SIZE_NUMBER]{2, 7, 6, 7, 2};
    alph['-'] = new int[SIZE_NUMBER]{4, 4, 8, 4, 4};
    alph['+'] = new int[SIZE_NUMBER]{4, 6, 8, 6, 4};
    alph['*'] = new int[SIZE_NUMBER]{4, 9, 6, 9, 4};
    alph['/'] = new int[SIZE_NUMBER]{4, 5, 6, 7, 4};
    alph['='] = new int[SIZE_NUMBER]{4, 0, 4, 0, 4};
    alph['C'] = new int[SIZE_NUMBER]{8, 2, 2, 2, 8};
    clearBuffer();
}

void Printer::putChar(char v) {
    int x = BUFFER_HEIGHT / 2 - SIZE_NUMBER / 2;
    int y = bufferSize;
    putCharInBuffer(x, y, v, maxScale);
}

void Printer::putInt(int v) {
    string number = to_string(v);
    int x = BUFFER_HEIGHT / 2 - SIZE_NUMBER / 2;
    int y = bufferSize;
    putStringInBuffer(x, y, number, maxScale);
}

void Printer::print() {
    for (int i = 0; i < BUFFER_HEIGHT; i++) {
        if (isSpaceLine(i))
            continue;
        for (int j = 0; j < BUFFER_WIDTH; j++) {
            cout << buffer[i][j];
        }
        cout << endl;
    }
}

void Printer::clear() {
    clearBuffer();
    bufferSize = 0;
    maxScale = 1;
}

int Printer::putStringInBuffer(int x, int y, std::string v, int scale) {
    int startY = y;
    for (int i = 0; i < v.size(); i++) {
        if (v[i] != 'C') {
            putCharInBuffer(x, y, v[i], scale);
            y += SIZE_NUMBER * scale;
            putSeparator(x, y);
            y++;
        } else {
            putCharInBuffer(x, y, 'C', scale);
            y += SIZE_NUMBER * scale;
            pair<string, string> comb = parseCombination(v.substr(i));
            int coef1 = comb.first.find("C") != comb.first.npos ? (SIZE_NUMBER / 2) * (scale - 1) : (scale - 1);
            int coef2 = comb.second.find("C") != comb.second.npos ? (SIZE_NUMBER / 2) * (scale - 1) : 2 * (scale - 1);
            int l1 = putStringInBuffer(x - SIZE_NUMBER / 2 * (scale - 1) - coef2, y, comb.second, scale - 1);
            int l2 = putStringInBuffer(x + SIZE_NUMBER + SIZE_NUMBER / 2 * (scale - 1) + coef1, y, comb.first,
                                       scale - 1);
            y += max(l1, l2);
            i += comb.first.size() + comb.second.size() + 3;
        }
    }
    return y - startY;
}

std::vector<std::vector<char>> Printer::activeBuffer() {
    vector<vector<char>> res;
    res.push_back(vector<char>(bufferSize + 2, ' '));
    for (int i = 0; i < BUFFER_HEIGHT; i++) {
        if (isSpaceLine(i))
            continue;
        vector<char> temp;
        temp.push_back(' ');
        for (int j = 0; j < bufferSize; j++) {
            temp.push_back(buffer[i][j]);
        }
        temp.push_back(' ');
        res.push_back(temp);
    }
    res.push_back(vector<char>(bufferSize + 2, ' '));
    return res;
}

void Printer::saveToBMP(std::string file) {
    std::ofstream os(file, std::ios::binary);

    vector<vector<char>> active = toScale(activeBuffer(), 3);
    unsigned int height = active.size();
    unsigned int width = active[0].size();

    unsigned char signature[2] = {'B', 'M'};
    unsigned int fileSize = 14 + 40 + height * width * 4;
    unsigned int reserved = 0;
    unsigned int offset = 14 + 40;

    unsigned int headerSize = 40;
    unsigned int dimensions[2] = {width, height};
    unsigned short colorPlanes = 1;
    unsigned short bpp = 32;
    unsigned int compression = 0;
    unsigned int imgSize = height * width * 4;
    unsigned int resolution[2] = {2795, 2795};
    unsigned int pltColors = 0;
    unsigned int impColors = 0;

    os.write(reinterpret_cast<char *>(signature), sizeof(signature));
    os.write(reinterpret_cast<char *>(&fileSize), sizeof(fileSize));
    os.write(reinterpret_cast<char *>(&reserved), sizeof(reserved));
    os.write(reinterpret_cast<char *>(&offset), sizeof(offset));

    os.write(reinterpret_cast<char *>(&headerSize), sizeof(headerSize));
    os.write(reinterpret_cast<char *>(dimensions), sizeof(dimensions));
    os.write(reinterpret_cast<char *>(&colorPlanes), sizeof(colorPlanes));
    os.write(reinterpret_cast<char *>(&bpp), sizeof(bpp));
    os.write(reinterpret_cast<char *>(&compression), sizeof(compression));
    os.write(reinterpret_cast<char *>(&imgSize), sizeof(imgSize));
    os.write(reinterpret_cast<char *>(resolution), sizeof(resolution));
    os.write(reinterpret_cast<char *>(&pltColors), sizeof(pltColors));
    os.write(reinterpret_cast<char *>(&impColors), sizeof(impColors));

    unsigned char x, r, g, b;

    for (int i = dimensions[1] - 1; i >= 0; --i) {
        for (int j = 0; j < dimensions[0]; ++j) {
            x = 0;
            if (active[i][j] == '#') {
                r = g = b = 0;
            } else {
                r = g = b = 255;
            }
            os.write(reinterpret_cast<char *>(&b), sizeof(b));
            os.write(reinterpret_cast<char *>(&g), sizeof(g));
            os.write(reinterpret_cast<char *>(&r), sizeof(r));
            os.write(reinterpret_cast<char *>(&x), sizeof(x));
        }
    }

    os.close();
}


void Printer::putString(string v) {
    v = replaceAll(v, " ", "");
    int x = BUFFER_HEIGHT / 2 - SIZE_NUMBER / 2;
    int y = bufferSize;
    maxScale = max(maxScale, countNesting(v));
    bufferSize += putStringInBuffer(x, y, v, maxScale);
}

int Printer::countNesting(std::string v) {
    int scale = 1;
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == 'C') {
            pair<string, string> comb = parseCombination(v.substr(i));
            scale += max(countNesting(comb.first), countNesting(comb.second));
            i += comb.first.size() + comb.second.size() + 3;
        }
    }
    return scale;
}

void Printer::putSeparator(int x, int y) {
    for (int i = x; i <= x + SIZE_NUMBER; i++) {
        for (int j = y; j <= y + SIZE_NUMBER; j++) {
            buffer[i][j] = ' ';
        }
    }
}

void Printer::putCharInBuffer(int x, int y, char ch, int scale) {
    if (alph.find(ch) == alph.end()) {
        return;
    }
    for (int i = 0; i < SIZE_NUMBER; i++) {
        for (int k = 0; k < scale; k++) {
            int y1 = y;
            for (int j = 0; j < SIZE_NUMBER; j++) {
                for (int t = 0; t < scale; t++) {
                    buffer[x][y1] = let[alph[ch][i]][j];
                    y1++;
                }
            }
            x++;
        }
    }
}

bool Printer::isSpaceLine(int x) {
    for (int i = 0; i < BUFFER_WIDTH; i++) {
        if (buffer[x][i] != ' ') {
            return false;
        }
    }
    return true;
}

void Printer::clearBuffer() {
    for (int i = 0; i < BUFFER_HEIGHT; i++) {
        for (int j = 0; j < BUFFER_WIDTH; j++) {
            buffer[i][j] = ' ';
        }
    }
}
