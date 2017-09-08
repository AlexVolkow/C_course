//
// Created by Alexandr Volkov on 13.03.2017.
//

#include <stack>
#include <stdlib.h>
#include <stdexcept>
#include <iostream>
#include "utils.h"
#include "expression.h"

using namespace std;


vector<string> Expression::to_polish(string s) {
    vector<string> res;
    stack<string> op;
    bool mayUnary = true;
    for (int i = 0; i < s.size(); i++) {
        if (isspace(s[i]) || s[i] == ',') {
            continue;
        }
        if (s[i] == '(') {
            op.push("(");
            mayUnary = true;
            continue;
        }
        if (s[i] == ')') {
            while (op.size() != 1 && op.top() != "(") {
                res.push_back(op.top());
                op.pop();
            }
            op.pop();
            mayUnary = false;
            continue;
        }
        if (!isdigit(s[i])) {
            string currOp(1, s[i]);
            if (priority(currOp) == 0) {
                throw logic_error("Incorrect expression");
            }
            if (mayUnary && currOp == "-") {
                currOp = "u" + currOp;
            }
            while (!op.empty() && priority(op.top()) >= priority(currOp)) {
                res.push_back(op.top());
                op.pop();
            }
            op.push(currOp);
            mayUnary = true;
        } else {
            int j = i + 1;
            while (j < s.size() && isdigit(s[j])) {
                j++;
            }
            res.push_back(s.substr(i, j - i));
            i = j - 1;
            mayUnary = false;
        }
    }
    while (!op.empty()) {
        res.push_back(op.top());
        op.pop();
    }
    return res;
}

int Expression::priority(string op) {
    if (op == "u-")
        return 5;
    if (op == "+" || op == "-")
        return 1;
    if (op == "*" || op == "/")
        return 2;
    if (op == "C")
        return 3;
    return 0;
}

Expression::Expression(string s) {
    if (check_brackets(s)) {
        this->expr = to_polish(prepare(s));
    } else {
        throw logic_error("Incorrect expression");
    }
}


string Expression::prepare(string s) {
    s = replaceAll(s, "c", "C");
    s = replaceAll(s, "C", "");
    s = replaceAll(s, ",", ")C(");
    return s;
}

BigInteger Expression::calc() {
    stack<BigInteger> stack;
    for (int i = 0; i < expr.size(); i++) {
        if (priority(expr[i]) != 0) {
            string op = expr[i];
            if (stack.empty()) {
                throw logic_error("Calculate exception");
            }
            BigInteger v1 = stack.top();
            stack.pop();
            if (op == "u-") {
                BigInteger top = -v1;
                stack.push(top);
                continue;
            }
            if (stack.empty()) {
                throw logic_error("Calculate exception");
            }
            BigInteger v2 = stack.top();
            stack.pop();
            if (op == "+") {
                stack.push(v1 + v2);
                continue;
            }
            if (op == "-") {
                stack.push(v2 - v1);
                continue;
            }
            if (op == "*") {
                stack.push(v1 * v2);
                continue;
            }
            if (op == "/") {
                if (v1 == 0) {
                    throw overflow_error("Divide by zero exception");
                }
                if (v1 > v2) {
                    stack.push(0);
                } else {
                    stack.push(v2 / v1);
                }
                continue;
            }
            if (op == "C") {
                if (v2 < v1 || v1 < 0 || v2 < 0) {
                    throw logic_error("Calculate exception");
                }
                if (!v1.isInt() && !v2.isInt()) {
                    throw logic_error("Calculate exception");
                }
                stack.push(BigInteger(combination(v2.intValue(), v1.intValue())));
            }
        } else {
            stack.push(BigInteger(expr[i]));
        }
    }
    if (stack.size() != 1) {
        throw logic_error("Incorrect expression");
    }
    return stack.top();
}

bool Expression::check_brackets(string s) {
    int open = 0;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '(') {
            open++;
            continue;
        }
        if (s[i] == ')') {
            if (open > 0) {
                open--;
            } else {
                return false;
            }
        }
    }
    return open == 0;
}
