//
// Created by Alexandr Volkov on 13.03.2017.
//

#ifndef PRINTER_EXPRESSION_H
#define PRINTER_EXPRESSION_H

#include <string>
#include <vector>
#include "biginteger.h"

class Expression {
private:
    std::vector<std::string> expr;

    std::vector<std::string> to_polish(std::string s);

    int priority(std::string op);

    bool check_brackets(std::string s);

    std::string prepare(std::string s);

public:
    Expression(std::string s);

    BigInteger calc();
};

#endif //PRINTER_EXPRESSION_H
